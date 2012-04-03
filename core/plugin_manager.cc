/**
 * plugin_manager.cc
 *
 *  Created on: Jul 5, 2011
 *      Author: Maros Kasinec
 */
#include "plugin_manager.h"

#ifndef USE_PRECOMPILED_HEADERS
#ifdef WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <dlfcn.h>
#endif
#endif

#include <iostream>
#include <sstream>

namespace sdc {

// PluginManager
PluginManager::PluginManager(Core* core)
    : AbstractManager(core),
      connection_provider_(this),
      connectionconfigmodel_provider_(this) {}
PluginManager::~PluginManager() {}

void PluginManager::LoadPlugins() {
  LOG(INFO) << "Loading plugins...";
  //TODO: loop it for a certain directory
  //TODO: implement try catch block
  std::string lib_filename = "libbuddycloud.so";
  std::string path = "/home/xangel/projects/social_desktop_client/plugins/";
  LIB_HANDLE_TYPE lib_handle = LoadLibrary(path + lib_filename);
  registerLibrary* fn_register = (registerLibrary*) LoadLibrarySymbol(lib_handle, "registerLibrary");
  Registration* registration = fn_register();
  LOG(INFO) << registration->name_ << " plugin loaded.";
  LibSignature lib_sig = GetLibrarySignature(lib_filename);
  lib_handles_[lib_sig] = lib_handle;
  for(std::vector<ClassDataRegistration>::iterator it = registration->new_plugins_.begin();
      it != registration->new_plugins_.end(); ++it) {
    it->lib_signature = lib_sig;
    class_data_[GetClassSignature(*it)] = (*it); // @anchor regFriend
  }
}

void PluginManager::UnloadPlugins() {
  LOG(INFO) << "Releasing plugins...";
  for(std::map<std::string, void*>::iterator i = lib_handles_.begin(); i != lib_handles_.end(); ++i) {
#ifdef WIN32
  FreeLibrary(i->second);
#else
  dlclose(i->second);
#endif
  }
}

LIB_HANDLE_TYPE PluginManager::LoadLibrary(const std::string & filepath) {
  // reset errors
  dlerror();
  LIB_HANDLE_TYPE lib_handle;
#ifdef WIN32
  lib_handle = LoadLibrary(TEXT(filepath.c_str()));
  if (!lib_handle) {
      std::cerr << "Cannot load library: " << TEXT(filepath.c_str()) << std::endl;
  }
#else
  lib_handle = dlopen(filepath.c_str(), RTLD_NOW);
  if (!lib_handle) {
      std::cerr << "Cannot load library: " << dlerror() << std::endl;
  }
#endif
  return lib_handle;
}

LIB_HANDLE_TYPE PluginManager::GetLibraryHandle(const PluginSignature & class_signature) {
  return lib_handles_[class_data_[class_signature].lib_signature];
}

void* PluginManager::LoadLibrarySymbol(LIB_HANDLE_TYPE lib_handle, const std::string & symbol) {
  // reset errors
  dlerror();
  // load symbol
  void* fn_handle = dlsym(lib_handle, symbol.c_str());
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol: " << dlsym_error << std::endl;
  }
  return fn_handle;
}

void* PluginManager::LoadLibrarySymbol(const PluginSignature & class_signature) {
  ClassDataRegistration* class_data = &class_data_[class_signature];
  return LoadLibrarySymbol(lib_handles_[class_data->lib_signature], class_data->class_symbol);
}

LibSignature PluginManager::GetLibrarySignature(const std::string & lib_filename) const {
  return lib_filename;
}

PluginSignature PluginManager::GetClassSignature(const ClassDataRegistration & class_data) const {
  std::ostringstream signature;
  signature << class_data.lib_signature << class_data.type << class_data.class_name;
  return signature.str();
}

std::vector<ClassDataRegistration> PluginManager::GetClassData(const PluginType & plugin_type) const {
  std::vector<ClassDataRegistration> filtered;
  for(std::map<PluginSignature, ClassDataRegistration>::const_iterator it = class_data_.begin();
      it != class_data_.end(); ++it) {
    if(it->second.type == plugin_type)
      filtered.push_back(it->second);
  }
  return filtered;
}

}
