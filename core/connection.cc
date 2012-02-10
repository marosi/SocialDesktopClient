/*
 * connection.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 31, 2012
 */

#include "connection.h"
#include "core.h"
#include "event_manager.h"

namespace sdc {

void Connection::DoRun() {
  is_active_ = true;
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION ACTIVE";
  Run();
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION INACTIVE";
  is_active_ = false;
}

void Connection::RecieveMessage(boost::shared_ptr<Message> message) {
  core()->event_manager()->PostEvent(boost::bind(&Core::Process, core(), message));
}

} /* namespace sdc */
