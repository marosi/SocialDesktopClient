/**
 * @file service_model.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 7, 2012
 */

#ifndef CORE_SERVICE_MODEL_H_
#define CORE_SERVICE_MODEL_H_

#include "model.h"
#include "core_anchor.h"

namespace sdc {

class AccountData;
class Connection;
class Service;

class ServiceModel : public Model, public CoreAnchor {
 public:
  friend class Core;

  ServiceModel() {} // TODO: REMOVE
  ServiceModel(AccountData* account) {
    account_ = account;
  }
  virtual ~ServiceModel() {}

  const AccountData* GetAccount() const {
    return account_;
  }

  Service* GetService() const {
    return service_;
  }

  virtual Connection* CreateConnection()=0;
  void Connect();
  void Disconnect();

 protected:
  AccountData* account_;
  Service* service_; // set in Core class
  Connection* connection_; // set in Core class
};

}  /* namespace sdc */

#endif /* CORE_SERVICE_MODEL_H_ */
