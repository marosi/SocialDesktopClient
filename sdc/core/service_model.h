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

class ServiceModel : public CoreAnchor {
 public:
  friend class Core;
  friend class ServiceManager;

  ServiceModel(Account* account);
  virtual ~ServiceModel() {}

  virtual void Run()=0;
  virtual void Stop()=0;
  virtual void Connect()=0;
  virtual void Disconnect()=0;

  Account* account() const { return account_; }
  Service* service() const { return service_; }

 private:
  void DoRun();

  Account* account_;
  Service* service_; // set in Core class
};

} // namespace sdc

#endif // CORE_SERVICE_MODEL_H_
