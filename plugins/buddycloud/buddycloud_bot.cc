/*
 * buddycloud_bot.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 28, 2012
 */

#include "buddycloud_bot.h"
#include "buddycloud_connection.h"
#include "boost/shared_ptr.hpp"

using namespace Swift;
using namespace boost;
using std::string;

BuddycloudBot::BuddycloudBot(BuddycloudConnection* connection, NetworkFactories* networkFactories) :
  connection_(connection) {
  //client = new Client("sdc_test@jabbim.com", "sdc_test", networkFactories);
  //client = new Client("maros@buddycloud.org", "udsampia", networkFactories);
  client = new Client("pista@localhost", "pista", networkFactories);
  client->setAlwaysTrustCertificates();
  client->onConnected.connect(bind(&BuddycloudBot::handleConnected, this));
  client->onMessageReceived.connect(
      bind(&BuddycloudBot::handleMessageReceived, this, _1));
  client->onPresenceReceived.connect(
      bind(&BuddycloudBot::handlePresenceReceived, this, _1));

  client->onDataRead.connect(bind(&BuddycloudBot::handleDataRecieved, this, _1));

  // Tracer
  tracer = new ClientXMLTracer(client);

  // Payload parsers and serializers
  client->addPayloadParserFactory(&pubsubPayloadParserFactory_);
  client->addPayloadSerializer(&pubsubPayloadSerializer_);

  client->getStanzaChannel()->onIQReceived.connect(bind(&BuddycloudBot::handleIQRecieved, this, _1));

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

void BuddycloudBot::SendDiscoInfo(const string &to_attribute, const string &node_attribute) {
  JID jid(to_attribute);
  GetDiscoInfoRequest::ref di = GetDiscoInfoRequest::create(jid, node_attribute, client->getIQRouter());
  di->send();
}

void BuddycloudBot::SendDiscoItems(const string &to_attribute, const string &node_attribute) {
  JID jid(to_attribute);
  GetDiscoItemsRequest::ref di = GetDiscoItemsRequest::create(jid, node_attribute, client->getIQRouter());
  di->send();
}

void BuddycloudBot::handleConnected() {
  LOG(DEBUG2) << "@@@ CLIENT IS CONNECTED @@@" << std::endl;
  // Request the roster
  GetRosterRequest::ref rosterRequest = GetRosterRequest::create(
      client->getIQRouter());
  rosterRequest->onResponse.connect(
      bind(&BuddycloudBot::handleRosterReceived, this, _2));
  rosterRequest->send();
}

void BuddycloudBot::handleIQRecieved(boost::shared_ptr<IQ> iq) {
  //shared_ptr<Body> payload = iq->getPayload<Body>();
  LOG(DEBUG2) << "IQ recieved"; // << payload->getText();
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
    //connection_->RecieveMessage(message->getBody());
  }
}

void BuddycloudBot::handlePresenceReceived(Presence::ref presence) {

  LOG(DEBUG) << "Handling presence message..";

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

void BuddycloudBot::handleDataRecieved(const SafeByteArray &byte_array) {
  /* WARNING! This breaks the safety of the data in the safe byte array.
     * Do not use in modes that require data safety. */
  connection_->RecieveMessage(safeByteArrayToString(byte_array));
}

