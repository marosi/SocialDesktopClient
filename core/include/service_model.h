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
#include "account_data.h"

namespace sdc {

class Connection;

class ServiceModel : public Model {
 public:
  ServiceModel() {} // TODO: REMOVE
  ServiceModel(AccountData* account_data) {
    account_ = account_data;
  }

  virtual Connection* CreateConnection()=0;

 protected:
  AccountData* account_;
};

}  /* namespace sdc */

#endif /* CORE_SERVICE_MODEL_H_ */
