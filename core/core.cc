/*
 * SocialDesktopClient.cc
 *
 *  Created on: Jul 8, 2011
 *      Author: Maros Kasinec
 */

#include "core.h"

namespace sdc {

// Manager global variables definition
ConfigManager* g_config_manager;
ConnectionManager* g_connection_manager;
PluginManager* g_plugin_manager;
EventManager* g_event_manager;
DataManager* g_data_manager;

Core::Core() {
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
  // execute application core in a non-blocking thread
  core_ = boost::thread(&Core::Exec, this);
}

void Core::Init() {
  g_config_manager = new ConfigManager;
  g_connection_manager = new ConnectionManager;
  g_plugin_manager = new PluginManager;
  g_event_manager = new EventManager;
  g_data_manager = new DataManager;
}

void Core::Exec() {
  // Set log reporting level
  Log::SetGlobalLevel(Log::DEBUG4);
  // Initialization
  g_plugin_manager->LoadPlugins();
  g_config_manager->Init();
  g_config_manager->LoadConfig("debug/configuration.conf");
  g_connection_manager->ConnectAll();

  g_data_manager->SetView(gui_);
  g_data_manager->ConnectView();
  // Main LOOP
  g_event_manager->Run();
  // Quit actions
  g_plugin_manager->UnloadPlugins();
}

void Core::Exit() {
  // stop event loop
  g_event_manager->Stop();
}

void Core::Join() {
  core_.join();
}

void Core::SetGui(void* gui) {
  gui_ = gui;
}

void* Core::GetGui() {
  return gui_;
}

}
