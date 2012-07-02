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
#include "mvc_typedefs.h"
#include "core_typedefs.h"
#include "boost/shared_ptr.hpp"

namespace sdc {

class Connection;
class ServiceController;
class ServiceModel;
class AccountData;

// @class Service
/// @brief TODO:
class Service {
 public:
  Service();

  std::string name() { return name_; }
  std::string description() { return description_; }
  /// @brief Factory method for user connection.
  virtual Connection* CreateConnection()=0;
  virtual ServiceModel* CreateServiceModel(AccountData*)=0;

  void SetSignature(std::string signature) {
    signature_ = signature;
  }

  std::string GetSignature() {
    return signature_;
  }

 protected:
  std::string signature_;
  std::string name_;
  std::string description_;
};

}

#endif /* CORE_SERVICE_H_ */
