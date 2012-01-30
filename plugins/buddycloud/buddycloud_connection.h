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

class BuddycloudBot;

class BuddycloudService : public sdc::Service {
 public:
  sdc::Service::Connection* CreateConnection();
  sdc::Service::UserConfig* CreateUserConfig();
};

class BuddycloudConnection : public sdc::Service::Connection {
 public:
  BuddycloudConnection();
  void Set(sdc::Service::UserConfig*);
  void Run();
  void SendMessage(boost::shared_ptr<sdc::Message> msg);
 private:
  BuddycloudBot* bot_;
};

class BuddycloudUserConfig : public sdc::Service::UserConfig {
 public:
  BuddycloudUserConfig(sdc::Service* service) : sdc::Service::UserConfig(service) { }
};

#endif /* BUDDYCLOUD_CONNECTION_H_ */
