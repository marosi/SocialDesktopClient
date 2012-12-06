/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_PLUGIN_MANAGER_H_
#define CORE_PLUGIN_MANAGER_H_

#ifdef WIN32
#define LIB_HANDLE_TYPE HINSTANCE
#else
#define LIB_HANDLE_TYPE void*
#endif

#include "abstract_manager.h"
#include "common.h"
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

namespace sdc {

class Service;
class PluginManager;

/**
 * Available plugin types.
 */
enum PluginType {
  SERVICE /**< Social network service implementation class */
};

/**
 * Structure holding data necessary for class registration in Core.
 */
struct ClassDataRegistration { // TODO: put ClassDataRegistration into Registration class
  PluginType type;
  std::string class_name;
  std::string class_symbol;
  LibSignature lib_signature;
};

/**
 * Plugin library registration object.
 * Provides interface for registering plugin metadata within SocialDesktopClient.
 */
class Registration {
 public:
  Registration(std::string & name, std::string & version, std::string & description)
      : name_(name), version_(version), description_(description) {}
  /// Each class that a plugin is extending is registered in Core by this method.
  void AddClass(PluginType type, const std::string & class_name, const std::string & class_symbol) {
    for(std::vector<ClassDataRegistration>::iterator it = new_plugins_.begin();
        it != new_plugins_.end(); ++it) {
      if(it->class_name == class_name)
        return; //TODO: Impelement error mechanism
    }
    ClassDataRegistration cd;
    cd.type = type;
    cd.class_name = class_name;
    cd.class_symbol = class_symbol;
    new_plugins_.push_back(cd);
  }
 private:
  friend class PluginManager; /// Just to pour out data immediately. @ref regFriend
  std::string name_;
  std::string version_;
  std::string description_;
  std::vector<ClassDataRegistration> new_plugins_;
};

/**
 * Low level plugin C function interface.
 * This function must be implemented in plugin library
 */
typedef Registration* registerLibrary();

/**
 * Factory class for creating plugin instances of available plugin types.
 */
template <class T>
class PluginProvider {
 public:
  PluginProvider(PluginManager* pm) : plugin_manager_(pm) {}
  typedef T* type();
  /**
   * Factory method. Creates object based on its plugin signature.
   */
  T* CreateInstance(const PluginSignature &);
 protected:
  PluginManager* plugin_manager_;
};


 /**
  * Takes care of plugin management, exposing plugin implementation to the Core.
  * It handles plugins represented by shared/dynamic libraries and provides interface for the work with plugins.
  */
class PluginManager : public AbstractManager {
 public:
  PluginManager(Core* core);
  ~PluginManager();
  void LoadPlugins(); /// Loads and registers every shared/dynamic library in a specified directory.
  void UnloadPlugins(); /// Frees all loaded libraries.

  LIB_HANDLE_TYPE GetLibraryHandle(const PluginSignature &);

  //@{ Loads symbol from shared/dynamic library
  void* LoadLibrarySymbol(LIB_HANDLE_TYPE, const std::string &);
  void* LoadLibrarySymbol(const PluginSignature &);
  //@}

  /**
   * Returns every class data object of a certain plugin type.
   */
  std::vector<ClassDataRegistration> GetClassData(const PluginType &) const;
  /**
   * Creates and returns all instances of plugin type.
   */
  template<class T>
  std::map<PluginSignature, T*> CreateAllInstances(const PluginType);

 private:
  /**
   * Loads library specified by its full path.
   */
  LIB_HANDLE_TYPE LoadLibrary(const std::string &);

  /**
   * Return library signature based on library file name
   * @
   * @return library signature
   */
  LibSignature GetLibrarySignature(const std::string &file_name) const;

  /**
   * Returns class signature based on class registration object.
   * @param class_data class registration structure
   * @return class signature
   */
  PluginSignature GetClassSignature(const ClassDataRegistration &class_data) const;

  PluginProvider<Service> connectionconfigmodel_provider_;
  std::map<PluginSignature, ClassDataRegistration> class_data_;
  std::map<LibSignature, LIB_HANDLE_TYPE> lib_handles_;
};

template<class T>
std::map<PluginSignature, T*> PluginManager::CreateAllInstances(const PluginType plugin_type) {
  PluginProvider<T> provider(this);
  std::vector<ClassDataRegistration> class_data = GetClassData(plugin_type);
  std::map<PluginSignature, T*> instances;
  T* inst;
  for(std::vector<ClassDataRegistration>::iterator it = class_data.begin();
      it != class_data.end(); ++it) {

    std::string signature = GetClassSignature(*it);
    inst = provider.CreateInstance(signature);
    instances.insert(std::pair<PluginSignature, T*>(signature, inst));
  }
  return instances;
}

template<class T>
T* PluginProvider<T>::CreateInstance(const PluginSignature & class_signature) {
  type* constructor = (type*) plugin_manager_->LoadLibrarySymbol(class_signature);
  return constructor();
}

} // namespace sdc

#endif // CORE_PLUGIN_MANAGER_H_
