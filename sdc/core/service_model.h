/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_SERVICE_MODEL_H_
#define CORE_SERVICE_MODEL_H_

#include "core_anchor.h"

namespace sdc {

class Account;
class Service;

/**
 * ServiceModel is a parent class of actual service business logic implementation.
 * Every service must inherit this class in order to be recognized by the Core.
 * It is dependend on service account information.
 */
class ServiceModel : public CoreAnchor {
 public:
  friend class Core;
  friend class ServiceManager;

  /**
   * Construct service object based on given account information.
   * @param account account data
   */
  ServiceModel(Account* account);
  virtual ~ServiceModel() {}

  /**
   * Service thread execution point.
   *
   * Since some services required long-lived connections,
   * it is desired to run their business logic in separte thread.
   */
  virtual void Run()=0;

  /**
   * Stops executing service thread.
   */
  virtual void Stop()=0;

  /**
   * Point of service network connecion.
   * Can perform actions like authentication, initial synchronization, etc.
   */
  virtual void Connect()=0;

  /**
   * Network disconnection from service.
   */
  virtual void Disconnect()=0;

  /**
   * Accessor method for the account.
   * @return account structure
   */
  Account* account() const { return account_; }

  /**
   * Accessor method the instance of respective service.
   * @return
   */
  Service* service() const { return service_; }

 private:
  void DoRun();

  Account* account_;
  Service* service_; // set in Core class
};

} // namespace sdc

#endif // CORE_SERVICE_MODEL_H_
