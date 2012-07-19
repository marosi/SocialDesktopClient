/*
 * service_model.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 20, 2012
 */

#include "service_model.h"
#include "account.h"

namespace sdc {

ServiceModel::ServiceModel(Account* account)
  : account_(account) {
  service_ = account->GetService();
}

void ServiceModel::DoRun() {
  LOG(TRACE) << "Service model for " << account_->GetId() << " is running.";
  Run();
  LOG(TRACE) << "Service model for " << account_->GetId() << " stopped.";
}

} /* namespace sdc */
