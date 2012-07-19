/**
 * @file plugin_manager.h
 * @brief SDC plugin interface.
 *
 * @author Maros Kasinec
 *   @date Jul 10, 2011
 */

#ifndef CORE_PLUGINMANAGER_H_
#define CORE_PLUGINMANAGER_H_

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

/// Possible implementation extensions of the SDC Core.
enum PluginType {
  SERVICE /// Social network service implementation class
};

/// Structure holding data necessary for class registration in Core.
struct ClassDataRegistration { // TODO: put ClassDataRegistration into Registration class
  PluginType type;
  std::string class_name;
  std::string class_symbol;
  LibSignature lib_signature;
};

/// Holds data that each plugin must provide.
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

/// @defgroup g1 Core-to-plugin interface
/// These C funcions provide communication between SDC Core and shared/dynamic libraries.
/// @{
typedef Registration* registerLibrary();
/// @}

template <class T>
class PluginProvider {
 public:
  PluginProvider(PluginManager* pm) : plugin_manager_(pm) {}
  typedef T* type();
  T* CreateInstance(const PluginSignature &); /// Factory method. Creates object based on its plugin signature.
 protected:
  PluginManager* plugin_manager_;
};

/// @class PluginManager
/// @brief Singleton class. Takes care of plugin management, exposing plugin implementation to SDC Core.
/// PluginManager acts as a bridge between SDC Core and its extensions.
/// It handles shared/dynamic libraries and provides interface for the work with plugins. It is accessible through sdc::g_plugin_manager.
/// It also works as class factory.
class PluginManager : public AbstractManager {
 public:
  PluginManager(Core* core);
  ~PluginManager();
  void LoadPlugins(); /// Loads and registers every shared/dynamic library in a specified directory.
  void UnloadPlugins(); /// Frees all loaded libraries.

  LIB_HANDLE_TYPE GetLibraryHandle(const PluginSignature &);

  /// @brief Loads symbol from shared/dynamic library
  void* LoadLibrarySymbol(LIB_HANDLE_TYPE, const std::string &);
  void* LoadLibrarySymbol(const PluginSignature &);

  /// @brief Returns every class data object of a certain plugin type.
  std::vector<ClassDataRegistration> GetClassData(const PluginType &) const;
  /// @brief Creates and returns all instances of plugin type.
  //template<class T>
  //std::vector<T>* CreateAllInstances(const PluginType);
  template<class T>
  std::map<PluginSignature, T*> CreateAllInstances(const PluginType);

 private:
  LIB_HANDLE_TYPE LoadLibrary(const std::string &); /// Loads library specified by its full path.
  LibSignature GetLibrarySignature(const std::string &) const;
  PluginSignature GetClassSignature(const ClassDataRegistration & class_data) const;

  PluginProvider<Service> connectionconfigmodel_provider_;
  std::map<PluginSignature, ClassDataRegistration> class_data_;
  std::map<LibSignature, LIB_HANDLE_TYPE> lib_handles_;
};

///
/// Template methods implementation
///
/*
template<class T>
std::vector<T>* PluginManager::CreateAllInstances(const PluginType plugin_type) {
  PluginProvider<T> plugin_provider(this); //TODO: !!!!
  std::vector<ClassDataRegistration> cd_array = GetClassData(plugin_type);
  std::vector<T>* instances = new std::vector<T>();
  for(std::vector<ClassDataRegistration>::iterator it = cd_array.begin();
      it != cd_array.end(); ++it)
    instances->push_back(*plugin_provider.CreateInstance(GetClassSignature(*it)));
  return instances;
}
*/

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

}

#endif /* CORE_PLUGINMANAGER_H_ */
