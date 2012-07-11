/*
 * connection_manager.cc
 *
 *  Created on: Aug 5, 2011
 *      Author: Maros Kasinec
 */

#include "connection_manager.h"
#include "connection.h"
#include "log.h"
#include "boost/foreach.hpp"
#include <exception>

using std::vector;
using boost::thread;

namespace sdc {

void ConnectionManager::MakeConnection(Connection* connection) {
  connections_.push_back(connection);
  try {
    thread* thrd = new thread(&Connection::DoRun, connection);
    threads_.add_thread(thrd);
  } catch (std::exception& e) {
    // TODO: handle exceptions
    LOG(ERROR) << "Problem in making connection thread:" << e.what();
  }
}

} /* namespace sdc */
