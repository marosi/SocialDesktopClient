/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "service_model.h"
#include "account.h"
#include "log.h"

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

} // namespace sdc
