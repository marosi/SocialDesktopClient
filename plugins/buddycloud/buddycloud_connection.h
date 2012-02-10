/*
 * buddycloud_connection.h
 *
 *  Created on: Jul 13, 2011
 *      Author: Maros Kasinec
 */
#ifndef BUDDYCLOUD_CONNECTION_H_
#define BUDDYCLOUD_CONNECTION_H_

#include "sdc.h"

class BuddycloudBot;

class BuddycloudConnection : public sdc::Connection {
 public:
  BuddycloudConnection();
  void Set(sdc::Service::UserConfig*);
  void Run();

  /// Testing @{
  void SendMessage(boost::shared_ptr<sdc::Message> msg);
  void SendMessage(const std::string &msg);
  void RecieveMessage(const std::string &msg);
  void HandleSendDiscoInfo(const std::string &to_attribute, const std::string &node_attribute);
  void HandleSendDiscoItems(const std::string &to_attribute, const std::string &node_attribute);
  void HandleSomething(const std::string &param);
  /// @}

 private:
  BuddycloudBot* bot_;
};

#endif /* BUDDYCLOUD_CONNECTION_H_ */
