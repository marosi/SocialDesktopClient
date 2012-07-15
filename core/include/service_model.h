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

#include "core_anchor.h"

namespace sdc {

class AccountData;
class Connection;
class Service;

class ServiceModel : public CoreAnchor {
 public:
  friend class Core;

  ServiceModel(AccountData* account);
  virtual ~ServiceModel() {}

  AccountData* account() const {
    return account_;
  }
  Service* service() const {
    return service_;
  }

  virtual Connection* CreateConnection()=0;
  virtual void Connect();
  virtual void Disconnect();

 protected:
  AccountData* account_;
  Service* service_; // set in Core class
  Connection* connection_; // set in Core class
};

}  /* namespace sdc */

#endif /* CORE_SERVICE_MODEL_H_ */
