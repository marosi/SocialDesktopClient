/*
 * SocialDesktopClient.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: Maros Kasinec
 */

#include "core.h"
#include "account_data.h"
#include "connection.h"
#include "log.h"
#include "service_model.h"
#include "ui.h"
#include "qt_gui.h"
// Managers
#include "config_manager.h"
#include "connection_manager.h"
#include "plugin_manager.h"
#include "event_manager.h"
// Boost
#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"
#include <algorithm>

#include "qt_service.h"
#include "boost/cast.hpp"

using namespace boost;
using namespace std;

namespace sdc {

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
 * Private interface
 */
void Core::ActivateAccount(AccountData* account) {
  Service* s = service(account->GetServiceSignature());
  account->SetService(s);
  ServiceModel* sam = s->CreateServiceModel(account);
  sam->SetCore(this);
  account->SetServiceModel(sam);
  service_models_.push_back(sam);
  account_models_.insert(AccountModelsMap::value_type(account->GetId(), sam));
  // connection
  Connection* conn = sam->CreateConnection();
  connections()->MakeConnection(conn);
  // hook model with its service and connection
  sam->service_ = account->GetService();
  sam->connection_ = conn;
  // emit signal
  onAccountActivated(account);
}

void Core::DeactivateAccount(AccountData* account) {
  ServiceModel* sam = account->GetServiceModel();
  if (!sam)
    return;
  account_models_.erase(account->GetId());
//  vector<Content::Ref> to_be_removed; // TODO: this is just a workaround to content-widget removal find more elegant solution
//  BOOST_FOREACH (Content::Ref c, contents_) {
//    if (c->GetServiceModel() == sam)
//      to_be_removed.push_back(c);
//  }
//  BOOST_FOREACH (Content::Ref c, to_be_removed) {
//    c->Remove();
//    contents_.erase(c);
//  }

  vector<ServiceModel*>::iterator it = find(service_models_.begin(), service_models_.end(), sam);
  //delete (*it);
  service_models_.erase(it);
  // emit signal
  onAccountDeactivated(account);
}

/**
 * Construct Core.
 */
Core::Core(int argc, char* argv[]) :
    is_gui_prepared_(false),
    ui_(new QtGui(this, argc, argv)) // Initialization point for specific GUI (Qt)
{
  Init();
}

Core::~Core() {
  delete ui_;
  delete config_manager_;
  delete connection_manager_;
  delete plugin_manager_;
  delete event_manager_;
}

void Core::Start() {
  // Set log reporting level
  Log::SetGlobalLevel(Log::DEBUG4);
  // Initialization
  plugins()->LoadPlugins();
  data()->Init();

  // Load Services
  services_ = plugin_manager_->CreateAllInstances<Service>(SERVICE); // TODO: Change method of getting instances from plugin manager
  std::map<PluginSignature, Service*>::iterator it = services_.begin();
  for(it = services_.begin(); it != services_.end(); ++it) {
    it->second->SetSignature(it->first);
    LOG(TRACE) << "Services class '" << it->first << "' loaded successfuly.";
  }

  // Map service object with loaded accounts
  for (AccountData* account : data()->accounts()) {
    if (services_.count(account->GetServiceSignature()) > 0) {
      Service* service = services_[account->GetServiceSignature()];
      account->SetService(service);
    }
  }

  data()->onAccountEnabled.connect(bind(&Core::ActivateAccount, this, _1));
  data()->onAccountDisabled.connect(bind(&Core::DeactivateAccount, this, _1));


  LOG(INFO) << "GUI(main) thread ID: " << boost::this_thread::get_id();
  // execute application core in a non-blocking thread
  core_ = boost::thread(&Core::Exec, this);

  // execute application UI in a blocking thread
  //ui_ = boost::thread(&Core::ExecUi, this);
  //ui_.join();
  ExecUi();

  // Quit actions
  plugin_manager_->UnloadPlugins();

  LOG(INFO) << "Exiting application...";
  Exit();
  core_.join();
}

void Core::Exit() {
  plugin_manager_->OnExit();
  connection_manager_->OnExit();
  config_manager_->OnExit();
  // stop event loop
  event_manager_->Stop();
}

void Core::Init() {
  config_manager_ = new ConfigManager(this);
  connection_manager_ = new ConnectionManager(this);
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
  for (AccountData* account : data()->accounts()) {
    if (account->IsEnabled()) {
      this->ActivateAccount(account);
    }
  }
  // connects services that were left online
  for (ServiceModel* model : service_models_) {
    if (model->GetAccount()->GetStatus()) // status == 0 is offline status
      model->Connect();
  }
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

} /* namespace sdc */
