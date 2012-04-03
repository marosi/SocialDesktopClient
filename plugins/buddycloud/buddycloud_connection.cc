/*
 * osw_connection.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Aug 9, 2011
 */

#include "buddycloud_connection.h"
#include "buddycloud_bot.h"
#include "b_controller.h"
#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"
#include "boost/pointer_cast.hpp"

using namespace sdc;
using std::string;
using boost::shared_ptr;

BuddycloudConnection::BuddycloudConnection() {
  LOG(DEBUG1) << "BuddycloudConnection has been instantiated.";
}

void BuddycloudConnection::Run() {
  //Swift::logging = true;

  Swift::SimpleEventLoop eventLoop;
  Swift::BoostNetworkFactories networkFactories(&eventLoop);

  bot_ = new BuddycloudBot(this, &networkFactories);

  eventLoop.run();
  LOG(DEBUG1) << "Connection running";
}

void BuddycloudConnection::Connect() {
  bot_->xmpp()->connect(); // TODO: implement more client with connect(Swift::ClientOptions*)
}

void BuddycloudConnection::Disconnect() {
  bot_->xmpp()->disconnect();
  LOG(TRACE) << "Disconnecting Swiften XMPP client.";
}

void BuddycloudConnection::OnConnected() {
  // TODO: is this method necessary?
}


/// Testing @{
void BuddycloudConnection::SendMessage(shared_ptr<sdc::Message> msg) {
  bot_->SendMessage(msg->GetText());
}

void BuddycloudConnection::SendMessage(const string &msg) {
  shared_ptr<Message> message = boost::make_shared<Message>(msg);
  SendMessage(message);
}

void BuddycloudConnection::RecieveMessage(const string &msg) {
  shared_ptr<Message> message = boost::make_shared<Message>(msg);
  Connection::RecieveMessage(message);
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
