/*
 * osw_connection.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Aug 9, 2011
 */

#include "buddycloud_connection.h"

#include "Swiften/Swiften.h"

#include "buddycloud_bot.h"

BuddycloudService::Connection::Connection() {
  LOG(DEBUG1) << "OswService::Connection has been instantiated.";
}
void BuddycloudService::Connection::Set(sdc::Service::UserConfig* uc) {
  LOG(DEBUG1) << "Setting user configuration";
}
void BuddycloudService::Connection::Run() {
  //Swift::logging = true;

  Swift::SimpleEventLoop eventLoop;
  Swift::BoostNetworkFactories networkFactories(&eventLoop);

  BuddycloudBot bot(&networkFactories);

  eventLoop.run();
  LOG(DEBUG1) << "Connection running";
}

sdc::Service::Connection* BuddycloudService::CreateConnection() { return new BuddycloudService::Connection(); }
sdc::Service::UserConfig* BuddycloudService::CreateUserConfig() { return new BuddycloudService::UserConfig(this); }
