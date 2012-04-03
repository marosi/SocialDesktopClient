/*
 * connection_manager.cc
 *
 *  Created on: Aug 5, 2011
 *      Author: Maros Kasinec
 */

#include "connection_manager.h"
#include "config_manager.h"
#include "connection.h"
#include "boost/foreach.hpp"
#include "boost/make_shared.hpp"
#include <exception>

using std::vector;
using boost::thread;

namespace sdc {

void ConnectionManager::InitServiceConnections() {
  vector<Account> accounts = g_config_manager->GetAccounts();
  vector<Account>::iterator u;
  for (u = accounts.begin(); u != accounts.end(); ++u) {
    ConnectionRef conn(u->GetService()->CreateConnection()); // TODO: Remove shared pointer from connection
    conn->SetService(u->GetService());
    // add new connection
    connections_.push_back(conn);
    // set connection anchor to Core
    conn->SetCore(GetCore());
  }
}

void ConnectionManager::ConnectAll() {
  vector<ConnectionRef>::iterator conn;
  for (conn = connections_.begin(); conn != connections_.end(); ++conn) {
    try {
      thread* thrd = new thread(&Connection::DoRun, *conn);
      threads_.add_thread(thrd);
      //threads_.join_all();
    } catch (std::exception& e) {
      // TODO: handle exceptions
      LOG(ERROR) << "Problem in making connection thread:" << e.what();
    }
  }
}

void ConnectionManager::GetAllActiveConnections(vector<ConnectionRef> &result) const {
  vector<ConnectionRef>::const_iterator it;
  for (it = connections_.begin(); it != connections_.end(); ++it) {
    ConnectionRef conn = *it;
    if(conn->IsActive())
      result.push_back(conn);
  }
}

} /* namespace sdc */
