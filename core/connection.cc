/*
 * connection.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 31, 2012
 */

#include "connection.h"
#include "commands.h"
#include "core.h"
#include "event_manager.h"

namespace sdc {

void Connection::DoRun() {
  is_active_ = true;
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION ACTIVE";
  this->Run();
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION INACTIVE";
  is_active_ = false;
}

void Connection::Send(RequestRef request) {
  pending_requests_.push_back(request);
  request->Send(this);
}

void Connection::DeleteRequest(RequestRef request) {
    pending_requests_.remove(request);
}

} /* namespace sdc */
