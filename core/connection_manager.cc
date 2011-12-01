/*
 * connection_manager.cc
 *
 *  Created on: Aug 5, 2011
 *      Author: Maros Kasinec
 */

#include "connection_manager.h"


#include <exception>

namespace sdc {

// ConnectionManager
void ConnectionManager::ConnectAll() {
  LOG(INFO) << "Connecting configured services...";
  typedef std::vector<Service::UserConfig*> UserConfigs;
  UserConfigs user_configs = g_config_manager->GetConnectionConfigs();
  for (UserConfigs::iterator u = user_configs.begin(); u != user_configs.end(); ++u) {
    MakeConnection(*u);
  }
}

void ConnectionManager::MakeConnection(Service::UserConfig* user_config) {
  try {
    Service::Connection* conn = user_config->CreateConnection();
    boost::thread* thrd = new boost::thread(&Service::Connection::Run, conn);
    threads_.add_thread(thrd);
    //threads_.join_all();
  } catch (std::exception& e) {
    // TODO: handle exceptions
    LOG(ERROR) << "Problem in making connection thread:" << e.what();
  }

}

} // namespace sdc
