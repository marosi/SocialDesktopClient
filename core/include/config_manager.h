/**
 * @file config_manager.h
 * @brief SDC configuration interface. User specific configuration data.
 *
 * @author Maros Kasinec
 *   @date Aug 6, 2011
 */

#ifndef CORE_CONFIGMANAGER_H_
#define CORE_CONFIGMANAGER_H_

#include "common.h"
#include "abstract_manager.h"
#include "service.h"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/vector.hpp"
#include <string>
#include <vector>
#include <map>

namespace sdc {

class Properties {
 public:
  friend class boost::serialization::access;

  void SetProperty(const std::string &key, const std::string &value) {
    props_[key] = value;
  }

  std::string GetProperty(const std::string &key) {
    return props_[key];
  }

 private:
  std::map<std::string, std::string> props_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(props_);
  }
};

class Library {
 public:
  friend class boost::serialization::access;

 private:
  bool enabled_;
  std::string filename_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
      ar & BOOST_SERIALIZATION_NVP(enabled_);
      ar & BOOST_SERIALIZATION_NVP(filename_);
  }
};

class Account {
 public:
  friend class boost::serialization::access;

  void SetUid(const std::string &uid) {
    uid_ = uid;
  }

  void SetPassword(const std::string &password) {
    password_ = password;
  }

  void SetServiceSignature(const PluginSignature &signature) {
    service_ = signature;
  }

  void SetService(Service* service) {
    service_ptr_ = service;
  }

  std::string GetUid() {
    return uid_;
  }

  std::string GetPassword() {
    return password_;
  }

  PluginSignature GetServiceSignature() {
    return service_;
  }

  Service* GetService() {
    return service_ptr_;
  }

 private:
  Service* service_ptr_;
  PluginSignature service_;
  std::string uid_;
  std::string password_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
      ar & BOOST_SERIALIZATION_NVP(service_);
      ar & BOOST_SERIALIZATION_NVP(uid_);
      ar & BOOST_SERIALIZATION_NVP(password_);
  }
};

/// @class ConfigManager
/// @brief Maintains application configuration and provides interface for other managers.
class ConfigManager : public AbstractManager, public Properties {
 public:
  friend class boost::serialization::access;
  ConfigManager(Core* core) : AbstractManager(core) {}
  void Init();

  void AddAccount(const Account &account) {
    accounts_.push_back(account);
  }

  const std::vector<Account> GetAccounts() const {
    return accounts_;
  }

  // TODO: Deprecated @{
  void LoadConfig(const std::string &);
  Service* GetService(const PluginSignature &/*signature*/);
  // @}

  void OnExit();

  //void RegisterConnection(ConnectionRegistration*); //HAHA 111st design approach
 private:
  static const std::string kConfFile;
  std::vector<Library> libraries_;
  std::vector<Account> accounts_;
  /*
   * Dynamic data
   */
  std::map<PluginSignature, Service*> services_; /// Pluged-in services and their configuration options

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
      //ar & boost::serialization::base_object<Properties>(*this);
      ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Properties);
      ar & BOOST_SERIALIZATION_NVP(libraries_);
      ar & BOOST_SERIALIZATION_NVP(accounts_);
  }
};

} /* namespace sdc */

#endif /* CORE_CONFIGMANAGER_H_ */
