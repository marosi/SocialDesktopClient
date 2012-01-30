/*
 * buddycloud_bot.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 28, 2012
 */

#include "buddycloud_bot.h"
#include "buddycloud_connection.h"
#include "boost/shared_ptr.hpp"

BuddycloudBot::BuddycloudBot(BuddycloudConnection* connection, NetworkFactories* networkFactories) :
  connection_(connection) {
  client = new Client("sdc_test@jabbim.com", "sdc_test", networkFactories);
  //client = new Client("maros@buddycloud.org", "udsampia", networkFactories);
  client->setAlwaysTrustCertificates();
  client->onConnected.connect(bind(&BuddycloudBot::handleConnected, this));
  client->onMessageReceived.connect(
      bind(&BuddycloudBot::handleMessageReceived, this, _1));
  client->onPresenceReceived.connect(
      bind(&BuddycloudBot::handlePresenceReceived, this, _1));
  tracer = new ClientXMLTracer(client);

  client->addPayloadParserFactory(&pubsubPayloadParserFactory_);
  client->addPayloadSerializer(&pubsubPayloadSerializer_);

  client->connect();
}

BuddycloudBot::~BuddycloudBot() {
  delete tracer;
  delete client;
}

void BuddycloudBot::SendMessage(std::string msg) {
  Message::ref message(new Message());
  message->setBody(msg);
  LOG(DEBUG2) << "Sending message: " << msg;
  client->sendMessage(message);
}

void BuddycloudBot::handleConnected() {
  std::cout << "@@@ CLIENT IS CONNECTED @@@" << std::endl;
  // Request the roster
  GetRosterRequest::ref rosterRequest = GetRosterRequest::create(
      client->getIQRouter());
  rosterRequest->onResponse.connect(
      bind(&BuddycloudBot::handleRosterReceived, this, _2));
  rosterRequest->send();
}

void BuddycloudBot::handleMessageReceived(Message::ref message) {

  LOG(DEBUG)
    << "Handling recieved message..";

  // Echo back the incoming message
  if (message->getBody() != "") {
    //std::cout << message->getBody() << std::endl;
    message->setTo(message->getFrom());
    message->setFrom(JID());
    //client->sendMessage(message);

    if (message->getPayload<PubsubPayload> ()) {

    }

    //boost::shared_ptr<IQ> iq = new boost::make_shared()
    //client->getIQRouter()->sendIQ()

    // Transform message into SDC core message format

    // Post message onto EventLoop queue
    boost::shared_ptr<sdc::Message> msg = boost::make_shared<sdc::Message>(message->getBody());
    connection_->RecieveMessage(msg);
  }
}

void BuddycloudBot::handlePresenceReceived(Presence::ref presence) {

  LOG(DEBUG)
    << "Handling presence message..";

  // Automatically approve subscription requests
  if (presence->getType() == Presence::Subscribe) {
    Presence::ref response = Presence::create();
    response->setTo(presence->getFrom());
    response->setType(Presence::Subscribed);
    client->sendPresence(response);
  }
}

void BuddycloudBot::handleRosterReceived(ErrorPayload::ref error) {
  if (error) {
    std::cerr << "Error receiving roster. Continuing anyway.";
  }
  // Send initial available presence
  client->sendPresence(Presence::create("Send me a message"));
}

