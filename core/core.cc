/*
 * SocialDesktopClient.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: Maros Kasinec
 */

#include "core.h"
#include "core_typedefs.h"
// MVC
#include "service_controller.h"
#include "service_model.h"
#include "service_view.h"
// Managers
#include "config_manager.h"
#include "connection_manager.h"
#include "plugin_manager.h"
#include "event_manager.h"
#include "data_manager.h"
//
#include "connection.h"
// UI
#include <QApplication>
#include "mainwindow.h"
// Boost
#include "boost/shared_ptr.hpp"
// Temporary test includes
#include "test_controller.h"
#include "test_model.h"
#include "testwindow.h"

namespace sdc {

// Manager global variables definition
ConfigManager* g_config_manager;
ConnectionManager* g_connection_manager;
PluginManager* g_plugin_manager;
EventManager* g_event_manager;
DataManager* g_data_manager;

Core::Core(int argc, char* argv[]) :
    is_gui_prepared_(false),
    qt_app_(new QApplication(argc, argv))
{
  Init();
}

Core::~Core() {
  delete main_view_;
  delete qt_app_;
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
  g_connection_manager->InitServiceConnections();
  g_connection_manager->ConnectAll();
  { // Core loop should not be started unless the GUI is prepared
    boost::unique_lock<boost::mutex> lock(mutex_);
    while(!is_gui_prepared_)
      gui_unprepared_.wait(lock);
  }
  g_event_manager->Run();
}

void Core::Exit() {
  // stop event loop
  g_event_manager->Stop();
}

void Core::RegisterController(ControllerRef controller) {
  controllers_.push_back(controller);
}

void Core::RegisterModel(ModelRef model) {
  models_.push_back(model);
}

void Core::Process(boost::shared_ptr<Message> message) {
  //test_controller_->PrintMessageFromPlugin(message);
}

void Core::InitUi() {
  // Init core models, controllers and views

  main_view_ = new MainWindow;
  //main_view_->show();

  // Launch GUI of each initiated service connection
  std::vector<ConnectionRef> conns;
  connection_manager()->GetAllActiveConnections(conns);
  std::vector<ConnectionRef>::iterator it;
  for(it = conns.begin(); it != conns.end(); ++it) {
    ConnectionRef conn = (*it);
    //conn->service()->InitializeGui(main_view_);
    // create service controller for connection
    ServiceControllerRef sctrler = conn->service()->CreateServiceController();
    conn->SetController(sctrler);
    sctrler->SetConnection(conn);
    // set controller's view and model
    ServiceViewRef sview = conn->service()->CreateMainView();
    ServiceModelRef smodel = conn->service()->CreateMainModel();
    sctrler->SetModel(smodel);
    sctrler->SetView(sview);
    sview->SetController(sctrler);
    sview->SetModel(smodel);
    //main_view_->StartUpServiceMainWidget(boost::dynamic_pointer_cast<QWidget>(view));
    sctrler->Initiate();
  }

  /*boost::shared_ptr<TestModel> tm = boost::make_shared<TestModel>();
  boost::shared_ptr<TestWindow> tw = boost::make_shared<TestWindow>();
  boost::shared_ptr<TestController> tc = boost::make_shared<TestController>();
  tc->SetModel(tm);
  tc->SetView(tw);
  tw->SetModel(tm);
  tw->SetController(tc);

  tw->show();

  test_controller_ = tc;*/
}

void Core::ExecUi() {
  InitUi();
  {
    boost::unique_lock<boost::mutex> lock(mutex_);
    is_gui_prepared_ = true;
  }
  gui_unprepared_.notify_one();
  return_code_ = qt_app_->exec();
}

}
