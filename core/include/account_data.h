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
#include "properties.h"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/vector.hpp"


#include "log.h"
namespace sdc {

class Service;
class ServiceModel;

class AccountData : public Properties {
 public:
  friend class boost::serialization::access;

  enum Status {
    Offline = 0,
    Online = 1
  };

  AccountData()
      : service_ptr_(0),
        service_model_ptr_(0) {}

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

  void SetStatus(int status) {
    status_ = status;
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

  int GetStatus() const {
    return status_;
  }

  std::string GetId() const {
    return service_ + uid_;
  }

  std::string GetDir() const {
    return "./resources/" + GetId();
  }

 private:
  Service* service_ptr_;  // service instance
  ServiceModel* service_model_ptr_; // service model instance
  int data_index_;  //

  PluginSignature service_;
  std::string uid_;
  std::string password_;
  bool is_enabled_;
  int status_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    if (version) {}
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Properties);
    ar & BOOST_SERIALIZATION_NVP(service_);
    ar & BOOST_SERIALIZATION_NVP(is_enabled_);
    ar & BOOST_SERIALIZATION_NVP(uid_);
    ar & BOOST_SERIALIZATION_NVP(password_);
    ar & BOOST_SERIALIZATION_NVP(status_);
  }
};

} /* namespace sdc */

#endif /* CORE_ACCOUNT_DATA_H_ */
