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

class BcModel;

class BuddycloudConnection : public sdc::Connection {
 public:
  BuddycloudConnection() {} // TODO: REMOVE
  BuddycloudConnection(BcModel* bot);

  void Run();
  void Connect();
  void Disconnect();

  BcModel* bot() {
    return bot_;
  }
 private:
  BcModel* bot_;
};

#endif /* BUDDYCLOUD_CONNECTION_H_ */
