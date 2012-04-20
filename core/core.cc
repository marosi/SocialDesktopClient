/*
 * SocialDesktopClient.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: Maros Kasinec
 */

#include "core.h"
#include "account_data.h"
#include "core_typedefs.h"
#include "connection.h"
#include "service_model.h"
#include "ui.h"
#include "qt_gui.h"
// Managers
#include "config_manager.h"
#include "connection_manager.h"
#include "plugin_manager.h"
#include "event_manager.h"
#include "data_manager.h"
// Boost
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"
#include "boost/bind.hpp"
#include <algorithm>

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

void Core::Process(boost::shared_ptr<Message> message) {
  //test_controller_->PrintMessageFromPlugin(message);
}

/**
 * Private interface
 */
void Core::ActivateAccount(AccountData* account) {
  ServiceModel* sam = account->GetService()->CreateServiceModel(account);
  account->SetServiceModel(sam);
  service_models_.push_back(sam);
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
  vector<ServiceModel*>::iterator it = find(service_models_.begin(), service_models_.end(), sam);
  service_models_.erase(it);
  // emit signal
  onAccountDeactivated(account);
}

/**
 * Core initialization
 */
// Manager global variables definition
ConfigManager* g_config_manager;
ConnectionManager* g_connection_manager;
PluginManager* g_plugin_manager;
EventManager* g_event_manager;
DataManager* g_data_manager;

Core::Core(int argc, char* argv[]) :
    is_gui_prepared_(false),
    ui_(new QtGui(this, argc, argv)) // Initialization point for specific GUI (Qt)
{
  Init();
}

Core::~Core() {
  delete ui_;
  delete g_config_manager;
  delete g_connection_manager;
  delete g_plugin_manager;
  delete g_event_manager;
  delete g_data_manager;
}

void Core::Start() {
  // Set log reporting level
  Log::SetGlobalLevel(Log::DEBUG4);
  // Initialization
  plugins()->LoadPlugins();
  data()->Init();


  // Load Services
  services_ = g_plugin_manager->CreateAllInstances<Service>(SERVICE); // TODO: Change method of getting instances from plugin manager
  std::map<PluginSignature, Service*>::iterator it = services_.begin();
  for(it = services_.begin(); it != services_.end(); ++it) {
    it->second->SetSignature(it->first);
    LOG(TRACE) << "Services class '" << it->first << "' loaded successfuly.";
  }

  // Map service object with loaded accounts
  BOOST_FOREACH (AccountData* account, data()->accounts()) {
    if (services_.count(account->GetServiceSignature()) > 0) {
      Service* service = services_[account->GetServiceSignature()];
      account->SetService(service);
    }
  }

  BOOST_FOREACH (AccountData* account, data()->accounts()) {
    if (account->IsEnabled()) {
      this->ActivateAccount(account);
    }
  }
  data()->onAccountEnabled.connect(bind(&Core::ActivateAccount, this, _1));
  data()->onAccountDisabled.connect(bind(&Core::DeactivateAccount, this, _1));

  BOOST_FOREACH (ServiceModel* model, service_models_) {
    if (model->GetAccount()->GetStatus()) // status == 0 is offline status
      model->Connect();
  }

  LOG(INFO) << "Main thread ID: " << boost::this_thread::get_id();
  // execute application core in a non-blocking thread
  core_ = boost::thread(&Core::Exec, this);

  // execute application UI in a blocking thread
  //ui_ = boost::thread(&Core::ExecUi, this);
  //ui_.join();
  ExecUi();

  // Quit actions
  g_plugin_manager->UnloadPlugins();

  LOG(INFO) << "Exiting application...";
  Exit();
  core_.join();
}

void Core::Exit() {
  g_data_manager->OnExit();
  g_plugin_manager->OnExit();
  g_connection_manager->OnExit();
  g_config_manager->OnExit();
  // stop event loop
  g_event_manager->Stop();
}

void Core::Init() {
  g_config_manager = new ConfigManager(this);
  g_connection_manager = new ConnectionManager(this);
  g_plugin_manager = new PluginManager(this);
  g_event_manager = new EventManager(this);
  g_data_manager = new DataManager(this);
}

void Core::Exec() {
  LOG(INFO) << "Core thread ID: " << boost::this_thread::get_id();
  { // Core loop should not be started unless the GUI is prepared
    boost::unique_lock<boost::mutex> lock(mutex_);
    while(!is_gui_prepared_)
      gui_unprepared_.wait(lock);
  }
  onGuiPrepared();
  g_event_manager->Run();
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
