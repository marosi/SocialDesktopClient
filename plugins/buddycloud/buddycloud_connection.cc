/*
 * osw_connection.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Aug 9, 2011
 */

#include "buddycloud_connection.h"
#include "buddycloud_bot.h"

#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"
#include "boost/pointer_cast.hpp"

BuddycloudConnection::BuddycloudConnection() {
  LOG(DEBUG1) << "OswService::Connection has been instantiated.";
}
void BuddycloudConnection::Set(sdc::Service::UserConfig* uc) {
  LOG(DEBUG1) << "Setting user configuration";
}
void BuddycloudConnection::Run() {
  //Swift::logging = true;

  Swift::SimpleEventLoop eventLoop;
  Swift::BoostNetworkFactories networkFactories(&eventLoop);

  bot_ = new BuddycloudBot(this, &networkFactories);

  eventLoop.run();
  LOG(DEBUG1) << "Connection running";
}

void BuddycloudConnection::SendMessage(boost::shared_ptr<sdc::Message> msg) {
  bot_->SendMessage(msg->GetText());
}

sdc::Service::Connection* BuddycloudService::CreateConnection() { return new BuddycloudConnection(); }
sdc::Service::UserConfig* BuddycloudService::CreateUserConfig() { return new BuddycloudUserConfig(this); }
