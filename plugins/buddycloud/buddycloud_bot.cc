/*
 * buddycloud_bot.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 28, 2012
 */

#include "buddycloud_bot.h"
#include "pubsub_requests.h"
#include "pubsub_parser.h"
#include "pubsub_serializer.h"
#include "boost/shared_ptr.hpp"

using namespace Swift;
using namespace boost;
using std::string;
using std::vector;

/**
 * PUBLIC INTERFACE
 */
BuddycloudBot::BuddycloudBot(const string &jid, const string &password)
    : jid_(jid),
      posts_node_("/user/" + jid_.toString() + "/posts") {
  //Swift::logging = true;
  loop_ = new SimpleEventLoop;
  network_ = new BoostNetworkFactories(loop_);
  //client_ = new Client("sdc_test@jabbim.com", "sdc_test", network_);
  //client_ = new Client("maros@buddycloud.org", "udsampia", network_);
  //client_ = new Client("pista@localhost", "pista", network_);
  //client_ = new Client("test_subject@buddycloud.org", "test", network_);
  client_ = new Client(jid_, password, network_);

  client_->setAlwaysTrustCertificates();
  client_->onConnected.connect(
      bind(&BuddycloudBot::handleConnected, this));
  client_->onMessageReceived.connect(
      bind(&BuddycloudBot::handleMessageReceived, this, _1));
  client_->onPresenceReceived.connect(
      bind(&BuddycloudBot::handlePresenceReceived, this, _1));
  client_->onDataRead.connect( // TODO :: remove
      bind(&BuddycloudBot::handleDataRecieved, this, _1));
  client_->getStanzaChannel()->onIQReceived.connect(
      bind(&BuddycloudBot::handleIQRecieved, this, _1));
  // Tracer
  tracer_ = new ClientXMLTracer(client_);
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
    //PublishAtomToNode(channel_user_.posts_node, atom);
  } else if (param == "items") {
    GetPubsubItemsRequest::ref req = GetPubsubItemsRequest::create(channel_service_.jid, channel_user_.posts_node, client_->getIQRouter());
    req->onResponse.connect(bind(&BuddycloudBot::handleItemsRecieved, this, _1));
    req->send();
  }
}

/**
 * PRIVATE INTERFACE
 */
void BuddycloudBot::handleItemsRecieved(boost::shared_ptr<PubsubItemsRequest> items) {
  LOG(DEBUG) << "handleItemsRecieved";
  LOG(DEBUG) << items->getItems()->get().at(0)->getAuthor();
  LOG(DEBUG) << items->getItems()->get().at(0)->getContent();
  LOG(DEBUG) << items->getItems()->get().at(0)->getObjectTypeString();
}

void BuddycloudBot::handleConnected() {
  LOG(TRACE) << "XMPP client is connected" << std::endl;
  // Request the roster
  GetRosterRequest::ref rosterRequest = GetRosterRequest::create(client_->getIQRouter());
  rosterRequest->onResponse.connect(
      bind(&BuddycloudBot::handleRosterReceived, this, _2));
  rosterRequest->send();

  // Discover channel service
  GetDiscoItemsRequest::ref items = GetDiscoItemsRequest::create(jid_.getDomain(), client_->getIQRouter());
  items->onResponse.connect(bind(&BuddycloudBot::handleServerDiscoItems, this, _1, _2));
  items->send();

  GetDiscoInfoRequest::ref itemss = GetDiscoInfoRequest::create(channel_service_.jid, "/user/pista@localhost", client_->getIQRouter());
  itemss->send();
}

void BuddycloudBot::handleIQRecieved(boost::shared_ptr<IQ> iq) {
  //shared_ptr<Body> payload = iq->getPayload<Body>();
  //LOG(DEBUG2) << "IQ recieved"; // << payload->getText();
  /*foreach (const shared_ptr<Payload> p, iq->getPayloads()) {
    LOG(DEBUG) << typeid(*p.get()).name();
  }*/
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
  }
}

void BuddycloudBot::handlePresenceReceived(Presence::ref presence) {
  LOG(DEBUG) << "Handling presence message..";
  // Automatically approve subscription requests
  if (presence->getType() == Presence::Subscribe) {
    LOG(INFO) << "Presence subscription request recieved -> approving subscription.";
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
  //LOG(DEBUG) << safeByteArrayToString(byte_array));
}

void BuddycloudBot::AddParserFactories() {
  // Payload parsers and serializers
  AddParserFactory(new GenericPayloadParserFactory<PubsubParser>("pubsub", "http://jabber.org/protocol/pubsub"));
}

void BuddycloudBot::AddSerializers() {
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

/**
 * Channel handling
 */
void BuddycloudBot::handleServerDiscoItems(shared_ptr<DiscoItems> payload, ErrorPayload::ref error) {
  LOG(TRACE) << "Handling server disco items ... ";
  vector<DiscoItems::Item>::const_iterator it = payload->getItems().begin();
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), client_->getIQRouter());
  info->onResponse.connect(bind(&BuddycloudBot::handleServerDiscoItemInfo, this, _1, _2, payload, it));
  info->send();
}

void BuddycloudBot::handleServerDiscoItemInfo(DiscoInfo::ref payload, ErrorPayload::ref error, shared_ptr<DiscoItems> items, vector<DiscoItems::Item>::const_iterator it) {
  if (error) {
    //LOG(DEBUG) << error->getText();
  } else {
    BOOST_FOREACH (const DiscoInfo::Identity &identity, payload->getIdentities()) {
      if (identity.getCategory() == "pubsub" && identity.getType() == "channels") {
        channel_service_.is_available = true;
        channel_service_.jid = it->getJID();
        if (payload->hasFeature("jabber:iq:register")) {
          channel_service_.is_registration_available = true;
        } else {
          channel_service_.is_registration_available = false;
        }
        this->UserChannelDiscovery();
      }
    }
  }
  if (!channel_service_.is_available) { // Go for next item if service is not found yet
    ++it;
    if (it == items->getItems().end()) {
      LOG(TRACE) << "Channel service is not available!";
      onError(ServiceUnavailable);
    } else {
      GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), client_->getIQRouter());
      info->onResponse.connect(bind(&BuddycloudBot::handleServerDiscoItemInfo, this, _1, _2, items, it));
      info->send();
    }
  }
}

void BuddycloudBot::UserChannelDiscovery() {
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(channel_service_.jid, posts_node_, client_->getIQRouter());
  info->onResponse.connect(bind(&BuddycloudBot::handleUserChannelDiscovery, this, _1, _2));
  info->send();
}

void BuddycloudBot::handleUserChannelDiscovery(DiscoInfo::ref payload, ErrorPayload::ref error) {
  if (error) {
    if (error->getCondition() == ErrorPayload::ItemNotFound) {
      LOG(WARNING) << "Jabber user '" << jid_ << "' has not registered channel yet. \n";
      if (channel_service_.is_registration_available) {
        LOG(TRACE) << "Trying to register user to service '" << channel_service_.jid.toString() << "'.";
        InBandRegistrationPayload::ref payload(new InBandRegistrationPayload);
        //payload->set... // TODO: Set user account data accordingly
        SetInBandRegistrationRequest::ref reg = SetInBandRegistrationRequest::create(channel_service_.jid, payload, client_->getIQRouter());
        reg->onResponse.connect(bind(&BuddycloudBot::handleChannelRegistration, this, _1, _2));
        reg->send();
      } else {
        LOG(ERROR) << "Registration to channel service '" << channel_service_.jid.toString() << "' is not available!";
      }
    }
  } else {
    BOOST_FOREACH (Form::ref form, payload->getExtensions()) {
      LOG(DEBUG) << "Node info form";
      LOG(DEBUG) << form->getField("pubsub#title")->getDescription();
      LOG(DEBUG) << form->getField("pubsub#title")->getLabel();
      LOG(DEBUG) << form->getField("pubsub#title")->getName();
      BOOST_FOREACH (string str, form->getField("pubsub#title")->getRawValues()) {
        LOG(DEBUG2) << "Values:";
        LOG(DEBUG2) << str;
      }
    }
  }
}

void BuddycloudBot::handleChannelRegistration(Payload::ref payload, ErrorPayload::ref error) {
  LOG(DEBUG) << "handleChannelREgistration";
}

/**
 * method is above is just for registration .. change its name
 * make method for geting node info ... struct node_info or GetNodeInfo(node)
 * subscribe SubscribeToNode(string node)
 * loook on MAM
 */


