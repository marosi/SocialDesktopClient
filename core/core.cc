/*
 * SocialDesktopClient.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: Maros Kasinec
 */

#include "core.h"
#include "core_typedefs.h"
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

namespace sdc {

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
  g_plugin_manager->LoadPlugins();
  g_config_manager->Init();
  g_config_manager->LoadConfig("debug/configuration.conf");
  g_connection_manager->InitServiceConnections();
  g_connection_manager->ConnectAll();

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

void Core::Init() {
  g_config_manager = new ConfigManager(this);
  g_connection_manager = new ConnectionManager(this);
  g_plugin_manager = new PluginManager(this);
  g_event_manager = new EventManager(this);
  g_data_manager = new DataManager(this);
}

void Core::Exec() {
  { // Core loop should not be started unless the GUI is prepared
    boost::unique_lock<boost::mutex> lock(mutex_);
    while(!is_gui_prepared_)
      gui_unprepared_.wait(lock);
  }
  g_event_manager->Run();
}

void Core::Exit() {
  g_data_manager->OnExit();
  g_plugin_manager->OnExit();
  g_connection_manager->OnExit();
  g_config_manager->OnExit();
  // stop event loop
  g_event_manager->Stop();
}

void Core::Process(boost::shared_ptr<Message> message) {
  //test_controller_->PrintMessageFromPlugin(message);
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
