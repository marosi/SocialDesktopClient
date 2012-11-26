/*
 * SocialDesktopClient.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: Maros Kasinec
 */

#include "core.h"
#include "account.h"
#include "log.h"
#include "service_model.h"
#include "ui.h"
#include "qt_gui.h"
#include "qt_service.h"
#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include "boost/filesystem.hpp"
#include "boost/foreach.hpp"
#include <algorithm>
#include <exception>

using namespace boost;
using namespace std;

namespace sdc {

std::string Core::home_dir("");

template std::map<PluginSignature, Service*> PluginManager::CreateAllInstances<Service>(const PluginType);

/**
 * Public interface
 */
std::vector<Service*> Core::services() {
  std::vector<Service*> result;
  std::map<PluginSignature, Service*>::iterator it;
  for(it = services_.begin(); it != services_.end(); ++it) {
    result.push_back(it->second);
  }
  return result;
}

Service* Core::service(const PluginSignature &signature) {
  return services_[signature];
}

/**
 * Construct Core.
 */
Core* Core::instance_ = NULL;

void Core::Create(UI* ui) {
  if (!instance_) {
    instance_ = new Core(ui);
  }
}

Core* Core::Instance() {
  return instance_;
}

Core::Core(UI* ui)
  : ui_(ui), is_gui_prepared_(false) {
  Init();
}

Core::~Core() {
  delete ui_;
  delete config_manager_;
  delete plugin_manager_;
  delete event_manager_;
}

void Core::Start() {
  /*
   * Set log reporting level
   */
  Log::SetGlobalLevel(Log::DEBUG4);
  /*
   * Initialization
   */
  plugins()->LoadPlugins();
  data()->Init();
  /*
   * Load Services
   */
  services_ = plugin_manager_->CreateAllInstances<Service>(SERVICE); // TODO: Change method of getting instances from plugin manager
  typedef std::pair<PluginSignature, Service*> PairType;
  BOOST_FOREACH (PairType pair , services_) {
    Service* s = pair.second;
    // set service unique signature
    s->signature_ = pair.first;
    // create service directory
    filesystem::path p(s->dir());
    if (!filesystem::exists(p)) {
      if (!filesystem::create_directory(p)) {
        LOG(ERROR) << "Cannot create directory for service '" << s->name() << "'.";
      }
    }
    LOG(TRACE) << "Services class '" << pair.first << "' loaded successfuly.";
  }
  /*
   * Map service object with loaded accounts
   */
  BOOST_FOREACH (Account* account , data()->accounts()) {
    if (services_.count(account->GetServiceSignature()) > 0) {
      Service* service = services_[account->GetServiceSignature()];
      account->SetService(service);
    }
  }
  /*
   * Bindings TODO: review
   */
  data()->onAccountEnabled.connect(bind(&Core::ActivateAccount, this, _1));
  data()->onAccountDisabled.connect(bind(&Core::DeactivateAccount, this, _1));
  /*
   * Execute application core in a non-blocking thread
   */
  core_ = boost::thread(&Core::Exec, this);
  /*
   * Execute application UI in a blocking thread
   */
  LOG(INFO) << "GUI(main) thread ID: " << boost::this_thread::get_id();
  ExecUi();
  /*
   * Quit actions
   */
  plugin_manager_->UnloadPlugins();
  LOG(INFO) << "Exiting application...";
  Exit();
  core_.join();
}

void Core::Exit() {
  plugin_manager_->OnExit();
  config_manager_->OnExit();
  // stop event loop
  event_manager_->Stop();
}

void Core::Init() {
  config_manager_ = new DataManager(this);
  plugin_manager_ = new PluginManager(this);
  event_manager_ = new EventManager(this);
}

void Core::Exec() {
  LOG(INFO) << "Core thread ID: " << boost::this_thread::get_id();
  { // Core loop should not be started unless the GUI is prepared
    boost::unique_lock<boost::mutex> lock(mutex_);
    while(!is_gui_prepared_)
      gui_unprepared_.wait(lock);
  }
  onGuiPrepared();
  // activate enabled accounts
  BOOST_FOREACH (Account* account , data()->accounts()) {
    if (account->IsEnabled()) {
      this->ActivateAccount(account);
    }
  }
  // TODO: connects services that were left online
//  BOOST_FOREACH (ServiceModel* model , service_models_) {
//    if (model->account()->GetStatus()) // status == 0 is offline status
//      model->Connect();
//  }
  // run event loop in core thread
  event_manager_->Run();
}

void Core::ExecUi() {
  ui_->Init();
  {
    boost::unique_lock<boost::mutex> lock(mutex_);
    is_gui_prepared_ = true;
  }
  gui_unprepared_.notify_one();
  return_code_ = ui_->Exec();
}


/**
 * ServiceModels handling
 */

void Core::ActivateAccount(Account* account) {
  // create account resource directory if none exists
  filesystem::path p(account->GetDir());
  if (!filesystem::exists(p)) {
    if (!filesystem::create_directory(p)) {
      LOG(ERROR) << "Cannot create directory for account '" << account->GetId() << "'.";
    }
  }
  // TODO: initialize account properly
  Service* s = service(account->GetServiceSignature());
  account->SetService(s);

  // create service model
  ServiceModel* model = s->CreateServiceModel(account);

  model->SetCore(this);
  account->SetServiceModel(model);

  service_models_.push_back(model);
  service_models_map_[account->GetId()] = model;

  MakeServiceThread(model);
  // hook model with its service and connection

  // signal activated account
  onAccountActivated(account->GetId());
}

void Core::DeactivateAccount(Account* account) {
  if (service_models_map_.count(account->GetId()) == 0)
    return;
  ServiceModel* model = service_models_map_[account->GetId()];
  model->Stop();
  model_threads_.remove_thread(model_threads_map_[model]);
  model_threads_map_.erase(model);
  service_models_map_.erase(account->GetId());
  vector<ServiceModel*>::iterator it = find(service_models_.begin(), service_models_.end(), model);
  //delete (*it);
  service_models_.erase(it);
  // signal deactivated account
  onAccountDeactivated(account->GetId());
}

void Core::MakeServiceThread(ServiceModel* model) {
  try {
    thread* thrd = new thread(&ServiceModel::DoRun, model);
    model_threads_map_[model] = thrd;
    model_threads_.add_thread(thrd);
    LOG(DEBUG) << "number of threads ... "<< model_threads_.size();
  } catch (std::exception& e) {
    LOG(ERROR) << "Problem when making service model thread:" << e.what();
  }
}

} /* namespace sdc */
