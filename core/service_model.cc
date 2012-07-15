/*
 * service_model.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 20, 2012
 */

#include "service_model.h"
#include "account_data.h"
#include "connection.h"

namespace sdc {

ServiceModel::ServiceModel(AccountData* account)
  : account_(account) {
  service_ = account->GetService();
}

void ServiceModel::Connect() {
  connection_->Connect();
}

void ServiceModel::Disconnect() {
  connection_->Disconnect();
}

} /* namespace sdc */
