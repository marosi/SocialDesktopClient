/*
 * buddycloud_connection.h
 *
 *  Created on: Jul 13, 2011
 *      Author: Maros Kasinec
 */
#ifndef BUDDYCLOUD_CONNECTION_H_
#define BUDDYCLOUD_CONNECTION_H_

#include <iostream>
#include "sdc.h"

class BuddycloudService : public sdc::Service {
 public:
  class Connection;
  class UserConfig;
  sdc::Service::Connection* CreateConnection();
  sdc::Service::UserConfig* CreateUserConfig();
};

class BuddycloudService::Connection : public sdc::Service::Connection {
 public:
  Connection();
  void Set(sdc::Service::UserConfig*);
  void Run();
};

class BuddycloudService::UserConfig : public sdc::Service::UserConfig {
 public:
  UserConfig(sdc::Service* service) : sdc::Service::UserConfig(service) { }
};

#endif /* BUDDYCLOUD_CONNECTION_H_ */
