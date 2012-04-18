/*
 * osw_connection.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Aug 9, 2011
 */

#include "buddycloud_connection.h"
#include "buddycloud_bot.h"
#include "b_controller.h"
#include "boost/shared_ptr.hpp"
#include "boost/pointer_cast.hpp"

using namespace sdc;
using std::string;
using boost::shared_ptr;

BuddycloudConnection::BuddycloudConnection(BuddycloudBot* bot)
    : bot_(bot) {
  LOG(DEBUG1) << "BuddycloudConnection has been instantiated.";
}

void BuddycloudConnection::Run() {
  LOG(INFO) << "Channel connection running";
  bot_->loop_->run();
}

void BuddycloudConnection::Connect() {
  bot_->client_->connect(); // TODO: implement more client with connect(Swift::ClientOptions*)
  LOG(TRACE) << "Connecting Swiften XMPP client.";
}

void BuddycloudConnection::Disconnect() {
  bot_->client_->disconnect();
  LOG(TRACE) << "Disconnecting Swiften XMPP client.";
}

/// Testing @{
void BuddycloudConnection::SendMessage(shared_ptr<sdc::Message> msg) {
  bot_->SendMessage(msg->GetText());
}

void BuddycloudConnection::SendMessage(const string &msg) {
  shared_ptr<sdc::Message> message = boost::make_shared<sdc::Message>(msg);
  SendMessage(message);
}

void BuddycloudConnection::HandleSendDiscoInfo(const string &to_attribute, const string &node_attribute) {
  bot_->SendDiscoInfo(to_attribute, node_attribute);
}

void BuddycloudConnection::HandleSendDiscoItems(const string &to_attribute, const string &node_attribute) {
  bot_->SendDiscoItems(to_attribute, node_attribute);
}

void BuddycloudConnection::HandleSomething(const string &param) {
  bot_->DoSomething(param);
}
/// @}
