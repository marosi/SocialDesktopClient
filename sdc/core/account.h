/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_ACCOUNT_H_
#define CORE_ACCOUNT_H_

#include "common.h"
#include "properties.h"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/vector.hpp"

namespace sdc {

class Service;
class ServiceModel;

/**
 * Main Account class holding common service account information.
 */
class Account : public Properties {
 public:
  friend class boost::serialization::access;

  Account()
      : service_ptr_(0),
        service_model_ptr_(0) {}

  virtual ~Account() {}

  /*
   * Setters
   */
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

  void SetEnabled(bool is_enabled) {
    is_enabled_ = is_enabled;
  }

  void SetServiceModel(ServiceModel* service_model) {
    service_model_ptr_ = service_model;
  }

  void SetDataIndex(int index) {
    data_index_ = index;
  }

  /*
   * Getters
   */
  std::string GetUid() const {
    return uid_;
  }

  std::string GetPassword() const {
    return password_;
  }

  PluginSignature GetServiceSignature() const {
    return service_;
  }

  Service* GetService() const {
    assert(service_ptr_);
    return service_ptr_;
  }

  bool IsEnabled() const {
    return is_enabled_;
  }

  ServiceModel* GetServiceModel() const {
    return service_model_ptr_;
  }

  int GetDataIndex() const {
    return data_index_;
  }

  std::string GetId() const {
    return service_ + uid_;
  }

  std::string GetDir() const {
    return "./resources/" + GetId();
  }

 private:
  Service* service_ptr_;  /**< actual service instance */
  ServiceModel* service_model_ptr_; /**< actual service model instance */
  int data_index_;  /**< index for presentational purposes */

  PluginSignature service_; /**< plugin service persistent signature */
  std::string uid_; /**< user's unique identifier */
  std::string password_; /**< pasword */ //TODO: secure
  bool is_enabled_; /**< specify account availability */

  /**
   * Method for account data serialization
   * conforming to Boost Serialization library guidelines.
   */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    if (version) {}
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Properties);
    ar & BOOST_SERIALIZATION_NVP(service_);
    ar & BOOST_SERIALIZATION_NVP(is_enabled_);
    ar & BOOST_SERIALIZATION_NVP(uid_);
    ar & BOOST_SERIALIZATION_NVP(password_);
  }
};

} // namespace sdc

#endif // CORE_ACCOUNT_H_
