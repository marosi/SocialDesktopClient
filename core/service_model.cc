/*
 * service_model.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 20, 2012
 */

#include "service_model.h"
#include "connection.h"

namespace sdc {

void ServiceModel::Connect() {
  connection_->Connect();
}

void ServiceModel::Disconnect() {
  connection_->Disconnect();
}

} /* namespace sdc */
