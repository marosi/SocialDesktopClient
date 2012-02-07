/*
 * connection_manager.cc
 *
 *  Created on: Aug 5, 2011
 *      Author: Maros Kasinec
 */

#include "connection_manager.h"
#include "config_manager.h"
#include "connection.h"
#include "boost/make_shared.hpp"
#include <exception>

using std::vector;
using boost::thread;

namespace sdc {

void ConnectionManager::InitServiceConnections() {
  vector<Service::UserConfig*> user_configs = g_config_manager->GetConnectionConfigs();
  vector<Service::UserConfig*>::iterator u;
  for (u = user_configs.begin(); u != user_configs.end(); ++u) {
    ConnectionRef conn((*u)->CreateConnection());
    // add new connection
    connections_.push_back(conn);
    // set connection anchor to Core
    conn->SetCore(GetCore());
  }
}

void ConnectionManager::ConnectAll() {
  LOG(INFO) << "Connecting configured services...";
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
