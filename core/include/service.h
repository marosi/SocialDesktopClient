/**
 * @file service.h
 * @brief TODO:comment
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Oct 28, 2011
 */

#ifndef CORE_SERVICE_H_
#define CORE_SERVICE_H_

#include "common.h"
#include "boost/shared_ptr.hpp"

namespace sdc {

class Connection;
class ServiceController;
class ServiceModel;
class AccountData;

/// @class Service
/// @brief TODO:
class Service {
 public:
  friend class Core;

  Service() : name_("Unknown"), description_("Unknown") {}

  std::string name() {
    return name_;
  }

  std::string description() {
    return description_;
  }

  const std::string dir() {
    return dir_path_;
  }

  std::string signature() {
    return signature_;
  }

  /// @brief Factory method for user connection.
  virtual Connection* CreateConnection()=0;
  virtual ServiceModel* CreateServiceModel(AccountData*)=0;

 protected:
  void SetName(const std::string &name) {
    name_ = name;
  }

  void SetDescription(const std::string &description) {
    description_ = description;
  }

  void SetDir(const std::string &dir_path) {
    dir_path_ = dir_path;
  }

 private:
  std::string signature_;
  std::string name_;
  std::string description_;
  std::string dir_path_;
};

}

#endif /* CORE_SERVICE_H_ */
