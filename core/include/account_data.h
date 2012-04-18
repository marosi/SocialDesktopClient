/**
 * @file account_data.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 14, 2012
 */

#ifndef CORE_ACCOUNT_DATA_H_
#define CORE_ACCOUNT_DATA_H_

#include "common.h"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/vector.hpp"

namespace sdc {

class Service;
class ServiceModel;

class AccountData {
 public:
  friend class boost::serialization::access;

  AccountData()
      : service_ptr_(0),
        service_model_ptr_(0) {}

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
    assert(service_model_ptr_);
    return service_model_ptr_;
  }

  int GetDataIndex() const {
    return data_index_;
  }

 private:
  Service* service_ptr_;  // service instance
  ServiceModel* service_model_ptr_; // service model instance
  int data_index_;  //

  PluginSignature service_;
  std::string uid_;
  std::string password_;
  bool is_enabled_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    if (version) {}
    ar & BOOST_SERIALIZATION_NVP(service_);
    ar & BOOST_SERIALIZATION_NVP(is_enabled_);
    ar & BOOST_SERIALIZATION_NVP(uid_);
    ar & BOOST_SERIALIZATION_NVP(password_);
  }
};

} /* namespace sdc */

#endif /* CORE_ACCOUNT_DATA_H_ */
