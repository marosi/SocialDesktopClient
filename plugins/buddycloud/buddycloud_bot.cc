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
using std::vector;

BuddycloudBot::BuddycloudBot(BuddycloudConnection* connection, NetworkFactories* networkFactories)
    : connection_(connection) {
  //client = new Client("sdc_test@jabbim.com", "sdc_test", networkFactories);
  //client = new Client("maros@buddycloud.org", "udsampia", networkFactories);
  client_ = new Client("pista@localhost", "pista", networkFactories); // TODO: take uid and password to SDC logic
  //client = new Client("test_subject@buddycloud.org", "test", networkFactories);
  client_->setAlwaysTrustCertificates();
  client_->onConnected.connect(bind(&BuddycloudBot::handleConnected, this));
  client_->onMessageReceived.connect(
      bind(&BuddycloudBot::handleMessageReceived, this, _1));
  client_->onPresenceReceived.connect(
      bind(&BuddycloudBot::handlePresenceReceived, this, _1));
  client_->onDataRead.connect( // TODO :: remove
      bind(&BuddycloudBot::handleDataRecieved, this, _1));
  // Tracer
  tracer_ = new ClientXMLTracer(client_);
  // Just playing
  client_->getStanzaChannel()->onIQReceived.connect(bind(&BuddycloudBot::handleIQRecieved, this, _1));
  // Add parsers and serializers
  AddParserFactories();
  AddSerializers();
}

BuddycloudBot::~BuddycloudBot() {
  for(vector<PayloadParserFactory*>::iterator
      it = parsers_.begin(); it != parsers_.end(); ++it) {
    client_->removePayloadParserFactory(*it);
    delete *it;
  }
  for (vector<PayloadSerializer*>::iterator
      it = serializers_.begin(); it != serializers_.end(); ++it) {
    client_->removePayloadSerializer(*it);
    delete *it;
  }
  delete tracer_;
  delete client_;
}

void BuddycloudBot::SendMessage(std::string msg) {
  Message::ref message(new Message());
  message->setBody(msg);
  LOG(DEBUG2) << "Sending message: " << msg;
  client_->sendMessage(message);
}

void BuddycloudBot::SendDiscoInfo(const string &to_attribute, const string &node_attribute) {
  JID jid(to_attribute);
  GetDiscoInfoRequest::ref di = GetDiscoInfoRequest::create(jid, node_attribute, client_->getIQRouter());
  di->send();
}

void BuddycloudBot::SendDiscoItems(const string &to_attribute, const string &node_attribute) {
  JID jid(to_attribute);
  GetDiscoItemsRequest::ref di = GetDiscoItemsRequest::create(jid, node_attribute, client_->getIQRouter());
  di->send();
}

void BuddycloudBot::DoSomething(const string &param) {
  LOG(DEBUG) << "Parameter from GUI : " << param;
  if (param == "gsv") {
    GetSoftwareVersionRequest::ref gsvr = GetSoftwareVersionRequest::create("localhost", client_->getIQRouter());
    gsvr->send();
  } else if (param == "gr") {
    GetRosterRequest::ref grr = GetRosterRequest::create(client_->getIQRouter());
    grr->send();
  } else if (param.substr(0, 3) == "gvc") {
    std::size_t pos = param.find('=');
    string to = param.substr(pos + 1);
    GetVCardRequest::ref gvcr = GetVCardRequest::create(to,
        client_->getIQRouter());
    gvcr->send();
  } else if (param.substr(0, 5) == "gibrf") {
    std::size_t pos = param.find('=');
    string to = param.substr(pos + 1);
    GetInBandRegistrationFormRequest::ref gibrf = GetInBandRegistrationFormRequest::create(to, client_->getIQRouter());
    gibrf->send();
  } else if (param.substr(0, 5) == "sibrf") {
    std::size_t pos = param.find('=');
    string to = param.substr(pos + 1);
    InBandRegistrationPayload::ref payload(new InBandRegistrationPayload);
    SetInBandRegistrationRequest::ref sibrf = SetInBandRegistrationRequest::create(to, payload, client_->getIQRouter());
    sibrf->send();
  } else if (param == "publish") {
    Atom::ref atom(new Atom);
    atom->setAuthor(channel_user_.jid);
    atom->setVerb(Atom::POST);
    atom->setObjectType(Atom::NOTE);
    atom->setContent("cuuuuuuuuzte vy tam vsetciiia");
    //
    PublishAtomToNode(channel_user_.posts_node, atom);
  } else if (param == "items") {
    GetPubsubItemsRequest::ref req = GetPubsubItemsRequest::create(channel_service_.jid, channel_user_.posts_node, client_->getIQRouter());
    req->onResponse.connect(bind(&BuddycloudBot::handleItemsRecieved, this, _1));
    req->send();
  }
}

void BuddycloudBot::handleItemsRecieved(boost::shared_ptr<PubsubItemsRequest> items) {
  LOG(DEBUG) << "handleItemsRecieved";
  LOG(DEBUG) << items->getItems()->get().at(0)->getAuthor();
  LOG(DEBUG) << items->getItems()->get().at(0)->getContent();
  LOG(DEBUG) << items->getItems()->get().at(0)->getObjectTypeString();
}

void BuddycloudBot::handleConnected() {
  LOG(DEBUG2) << "@@@ CLIENT IS CONNECTED @@@" << std::endl;
  // Request the roster
  GetRosterRequest::ref rosterRequest = GetRosterRequest::create(client_->getIQRouter());
  rosterRequest->onResponse.connect(
      bind(&BuddycloudBot::handleRosterReceived, this, _2));
  rosterRequest->send();
  // Discover channel service
  DiscoverChannelService();
  // Discover user channel
  DiscoverUserSelfChannel();
  connection_->OnConnected();
}

void BuddycloudBot::handleIQRecieved(boost::shared_ptr<IQ> iq) {
  //shared_ptr<Body> payload = iq->getPayload<Body>();
  LOG(DEBUG2) << "IQ recieved"; // << payload->getText();
  foreach (const shared_ptr<Payload> p, iq->getPayloads()) {
    LOG(DEBUG) << typeid(*p.get()).name();
  }
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

    /*if (message->getPayload<PubsubPayload> ()) {

    }*/

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
    client_->sendPresence(response);
  }
}

void BuddycloudBot::handleRosterReceived(ErrorPayload::ref error) {
  if (error) {
    std::cerr << "Error receiving roster. Continuing anyway.";
  }
  // Send initial available presence
  client_->sendPresence(Presence::create("Send me a message"));
}

void BuddycloudBot::handleDataRecieved(const SafeByteArray &byte_array) {
  /* WARNING! This breaks the safety of the data in the safe byte array.
     * Do not use in modes that require data safety. */
  connection_->RecieveMessage(safeByteArrayToString(byte_array));
}

void BuddycloudBot::AddParserFactories() {
  // Payload parsers and serializers
  AddParserFactory(new GenericPayloadParserFactory<PubsubParser>("pubsub", "http://jabber.org/protocol/pubsub"));
}

void BuddycloudBot::AddSerializers() {
  AddSerializer(new PubsubSerializer);
  AddSerializer(new PubsubItemsRequestSerializer);
  AddSerializer(new PubsubRetractRequestSerializer);
  AddSerializer(new PubsubPublishRequestSerializer);
}

void BuddycloudBot::AddParserFactory(Swift::PayloadParserFactory* factory) {
  parsers_.push_back(factory);
  client_->addPayloadParserFactory(factory);
}

void BuddycloudBot::AddSerializer(Swift::PayloadSerializer* serializer) {
  serializers_.push_back(serializer);
  client_->addPayloadSerializer(serializer);
}

