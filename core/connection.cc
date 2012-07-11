/*
 * connection.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 31, 2012
 */

#include "connection.h"
#include "core.h"
#include "log.h"
#include "event_manager.h"

namespace sdc {

void Connection::DoRun() {
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION ACTIVE";
  this->Run();
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION INACTIVE";
}

} /* namespace sdc */
