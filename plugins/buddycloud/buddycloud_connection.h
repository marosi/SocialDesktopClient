/*
 * buddycloud_connection.h
 *
 *  Created on: Jul 13, 2011
 *      Author: Maros Kasinec
 */
#ifndef BUDDYCLOUD_CONNECTION_H_
#define BUDDYCLOUD_CONNECTION_H_

#include "sdc.h"
#include "Swiften/Swiften.h"

class BuddycloudBot;

class BuddycloudConnection : public sdc::Connection {
 public:
  BuddycloudConnection() {} // TODO: REMOVE
  BuddycloudConnection(BuddycloudBot* bot);

  void Run();
  void Connect();
  void Disconnect();

  BuddycloudBot* bot() {
    return bot_;
  }
 private:
  BuddycloudBot* bot_;
};

#endif /* BUDDYCLOUD_CONNECTION_H_ */
