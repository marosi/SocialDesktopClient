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
  vector<AccountData*> accounts = g_config_manager->GetAccounts();
  vector<AccountData*>::iterator u;
  for (u = accounts.begin(); u != accounts.end(); ++u) {
    ConnectionRef conn((*u)->GetService()->CreateConnection()); // TODO: Remove shared pointer from connection
    // add new connection
    connections_old.push_back(conn);
    // set connection anchor to Core
    conn->SetService((*u)->GetService());
    conn->SetCore(GetCore());
  }
}

void ConnectionManager::ConnectAll() {
  vector<ConnectionRef>::iterator conn;
  for (conn = connections_old.begin(); conn != connections_old.end(); ++conn) {
    MakeConnection((*conn).get());
  }
}

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

void ConnectionManager::GetAllActiveConnections(vector<ConnectionRef> &result) const {
  vector<ConnectionRef>::const_iterator it;
  for (it = connections_old.begin(); it != connections_old.end(); ++it) {
    ConnectionRef conn = *it;
    if(conn->IsActive())
      result.push_back(conn);
  }
}

} /* namespace sdc */
