/*
 * SocialDesktopClient.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: Maros Kasinec
 */

#include "core.h"
// MVC
#include "controller.h"
#include "model.h"
// Managers
#include "config_manager.h"
#include "connection_manager.h"
#include "plugin_manager.h"
#include "event_manager.h"
#include "data_manager.h"
// UI
#include "mainwindow.h"
// Boost
#include "boost/shared_ptr.hpp"
// Temporary test includes
#include "test_controller.h"
#include "test_model.h"

#include "newwindow.h"

namespace sdc {

// Manager global variables definition
ConfigManager* g_config_manager;
ConnectionManager* g_connection_manager;
PluginManager* g_plugin_manager;
EventManager* g_event_manager;
DataManager* g_data_manager;

Core::Core(int argc, char* argv[]) : qt_app_(argc, argv) {
  Init();
}

Core::~Core() {
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
  g_plugin_manager->LoadPlugins();
  g_config_manager->Init();
  g_config_manager->LoadConfig("debug/configuration.conf");

  // execute application core in a non-blocking thread
  core_ = boost::thread(&Core::Exec, this);

  // execute application UI in a non-blocking thread
  //ui_ = boost::thread(&Core::ExecUi, this);
  //ui_.join();
  ExecUi();

  // Quit actions
  g_plugin_manager->UnloadPlugins();

  LOG(INFO) << "Exiting application...";
  Exit();
  core_.join();
}

void Core::Init() {
  g_config_manager = new ConfigManager;
  g_connection_manager = new ConnectionManager;
    g_connection_manager->SetCore(this);
  g_plugin_manager = new PluginManager;
  g_event_manager = new EventManager;
    g_event_manager->SetCore(this);
  g_data_manager = new DataManager;
}

void Core::Exec() {
  g_connection_manager->ConnectAll();
  // Main LOOP
  g_event_manager->Run();
}

void Core::Exit() {
  // stop event loop
  g_event_manager->Stop();
}

void Core::RegisterController(Controller::Ref controller) {
  controllers_.push_back(controller);
}

void Core::RegisterModel(Model::Ref model) {
  models_.push_back(model);
}

void Core::Process(boost::shared_ptr<Message> message) {
  test_controller_->PrintMessageFromPlugin(message);
}

void Core::InitUi() {
  // Init core models, controllers and views

  main_view_ = new MainWindow;
  main_view_->show();

  //boost::shared_ptr<NewWindow> nw = boost::make_shared<NewWindow>();
  //nw->show();

  boost::shared_ptr<TestModel> tm = boost::make_shared<TestModel>();
  //RegisterModel(tm);
  boost::shared_ptr<TestController> tc = boost::make_shared<TestController>(tm);
  tc->Init(); //initialize controller ... it will create also view TestWindow which registers to model
  //RegisterController(tc);
  test_controller_ = tc;
  // Init pluged-in models, controllers and views

}

void Core::ExecUi() {
  InitUi();
  return_code_ = qt_app_.exec();
}

}
