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
using namespace sdc;
using std::string;
using std::vector;

/*class BcClient : public Swift::Client {
 public:
  BcClient(const JID& jid, const SafeString& password, NetworkFactories* networkFactories, Storages* storages = NULL)
      : Swift::Client(jid, password, networkFactories, storages) {

      getStanzaChannel()->onAvailableChanged.connect(bind(&BcClient::handleAvailable, this, _1));
  }

 private:
  void handleSessionInitialized() {
    LOG(DEBUG) << "Session is initialized.";
  }

  void handleNeedCredentials() {
    LOG(DEBUG) << "Session need credentials.";
  }

  void handleFinished(boost::shared_ptr<Swift::Error> error) {
    if (error)
    LOG(DEBUG) << "some error aaaaaaaaaaaaaaaaaaaaaaaaaa";
    else
    LOG(DEBUG) << "FINISHEEEEEEEEEEEEEEEEEEED";
  }

  void handleAvailable(bool is_available) {

    // not working
    //getSession()->onNeedCredentials.disconnect(boost::bind(&CoreClient::handleNeedCredentials, this));

    LOG(DEBUG) << "STANZA CHANNEL IS " << is_available;
    getSession()->onInitialized.connect(bind(&BcClient::handleSessionInitialized, this));
    getSession()->onNeedCredentials.connect(bind(&BcClient::handleNeedCredentials, this));

    getSession()->onFinished.connect(bind(&BcClient::handleFinished, this, _1));

  }

};*/

/**
 * PUBLIC INTERFACE
 */
BuddycloudBot::BuddycloudBot(sdc::AccountData* account)
    : QtServiceModel(account),
      my_channel_(0),
      jid_(account->GetUid()),
      posts_node_("/user/" + jid_.toString() + "/posts"),
      connected_(false) {
  LOG(DEBUG) << "This is buddycloud Service Model!";
  //Swift::logging = true;
  loop_ = new SimpleEventLoop;
  network_ = new BoostNetworkFactories(loop_);
  //client_ = new Client("sdc_test@jabbim.com", "sdc_test", network_);
  //client_ = new Client("maros@buddycloud.org", "udsampia", network_);
  //client_ = new Client("pista@localhost", "pista", network_);
  //client_ = new Client("test_subject@buddycloud.org", "test", network_);
  client_ = new Client(jid_, account->GetPassword(), network_);

  client_->setAlwaysTrustCertificates();
  client_->onConnected.connect(
      bind(&BuddycloudBot::handleConnected, this));
  client_->onDisconnected.connect(
      bind(&BuddycloudBot::handleDisconnected, this, _1));
  client_->onMessageReceived.connect(
      bind(&BuddycloudBot::handleMessageReceived, this, _1));
  client_->onPresenceReceived.connect(
      bind(&BuddycloudBot::handlePresenceReceived, this, _1));
  client_->onDataRead.connect( // TODO :: remove
      bind(&BuddycloudBot::handleDataRecieved, this, _1));
  client_->getStanzaChannel()->onIQReceived.connect(
      bind(&BuddycloudBot::handleIQRecieved, this, _1));


  client_->getRoster()->onJIDAdded.connect(bind(&BuddycloudBot::handleJidAdded, this, _1));
  client_->getRoster()->onJIDRemoved.connect(bind(&BuddycloudBot::handleJidRemove, this, _1));
  client_->getRoster()->onJIDUpdated.connect(bind(&BuddycloudBot::handleJidUpdated, this, _1, _2, _3));

  // Tracer
  tracer_ = new ClientXMLTracer(client_);
  // Add parsers and serializers
  AddParserFactories();
  AddSerializers();
}


void BuddycloudBot::GetContactChannel(Contact::Ref contact) {
  Channel::Ref channel;
  BOOST_FOREACH (ChannelController* ch, channel_controllers_) {
    LOG(DEBUG) << "channel... " << ch->posts_node_;
    if (ch->contact_ == contact)
      channel = ch->channel_;
  }
LOG(DEBUG) << "HEREEEEE";
  if (channel) {
    LOG(DEBUG) << "34180237481274091234901234 " << channel->GetTitle();
    core()->PushContent(this, channel);
  } else {
    ChannelController* channel = new ChannelController(contact, this);
    channel_controllers_.insert(channel);
    channel->onReady.connect(bind(&BuddycloudBot::handleContactChannel, this, _1));
    channel->Start();
  }
}

void BuddycloudBot::AddNewContact(BcContact::Ref contact) {
  // TODO: new channel contact handling
  if (BcContact::Ref c = GetContact(contact->GetUid())) {
  } else {

    RosterItemPayload item;
    item.setJID(contact->GetUid());
    item.setSubscription(RosterItemPayload::To);
    item.setSubscriptionRequested();
    RosterPayload::ref roster(new RosterPayload);
    roster->addItem(item);
    SetRosterRequest::ref set = SetRosterRequest::create(roster, client_->getIQRouter());
    set->send();

  }
}

void BuddycloudBot::RemoveContact(BcContact::Ref contact) {
  /*Presence::ref presence(new Presence);
  presence->setTo(contact->GetUid());
  presence->setType(Presence::Unsubscribe);
  client_->sendPresence(presence);*/

  RosterPayload::ref roster(new RosterPayload);
  RosterItemPayload item;
  item.setJID(contact->GetUid());
  item.setSubscription(RosterItemPayload::Remove);
  roster->addItem(item);
  SetRosterRequest::ref set = SetRosterRequest::create(roster, client_->getIQRouter());
  set->send();
}

void BuddycloudBot::handleJidAdded(const JID &jid) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been added to roster";
  XMPPRosterItem item = client_->getRoster()->getItem(jid).get();
  BcContact::Ref contact(new BcContact);
  contact->SetBot(this);
  contact->SetUid(item.getJID());
  contact->SetName(item.getName());
  contact->SetGroups(item.getGroups());
  contact->SetViewable(true);
  contacts_.push_back(contact);
  core()->PushContent(this, contact);
  GetContactChannel(contact);
}

void BuddycloudBot::handleJidRemove(const JID &jid) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been removed from the roster";
  vector<BcContact::Ref>::iterator it;
  for (it = contacts_.begin(); it != contacts_.end(); ++it) {
    LOG(DEBUG) << "contact ..." << (*it)->GetUid();
    if ((*it)->GetUid() == jid.toString()) {
      LOG(DEBUG) << "Is in there!!!!!!!!!!!!!!!!!!!!!!!!!";
      break;
    }
  }
  if (it != contacts_.end()) {
    core()->RemoveContent(*it);
    contacts_.erase(it);
  }
}

void BuddycloudBot::handleJidUpdated(const JID &jid, const string &name, const vector<string> &groups) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been updated in roster";
}

BuddycloudBot::~BuddycloudBot() {
  client_->disconnect();

  LOG(TRACE) << "Buddycloud service model is destroying";
  BOOST_FOREACH (BcContact::Ref &contact, contacts_) {
    core()->RemoveContent(contact);
  }

  if (my_channel_)
    delete my_channel_;

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
    //req->onResponse.connect(bind(&BuddycloudBot::handleItemsRecieved, this, _1));
    req->send();
  }
}

BcContact::Ref BuddycloudBot::GetContact(const Swift::JID &jid) {
  BOOST_FOREACH (BcContact::Ref &c, contacts_) {
    if (c->GetUid() == jid.toString())
      return c;
  }
  return BcContact::Ref();
}

Post1::Ref BuddycloudBot::TranslateAtom(const Atom::ref atom) {
  Post1::Ref post(new Post1);
  post->SetID(atom->getID());
  post->SetAuthor(atom->getAuthor());
  post->SetContent(atom->getContent());
  return post;
}

void BuddycloudBot::GetServerInfo() {
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(jid_.getDomain(), client_->getIQRouter());
  info->onResponse.connect(bind(&BuddycloudBot::handleServerInfo, this, _1, _2));
  info->send();
}

void BuddycloudBot::handleServerInfo(DiscoInfo::ref payload, ErrorPayload::ref error) {
  if (!error)
    server_info_ = payload;
}

void BuddycloudBot::handleConnected() {
  LOG(TRACE) << "XMPP client is connected" << std::endl;

  // Send presence message
  client_->sendPresence(Presence::create("Ready when you are..."));

  if (!me_) {
    me_ = boost::make_shared<Contact>();
    me_->SetUid(jid_.toString());
    my_channel_ = new ChannelController(me_, this);
    my_channel_->onChannelsServiceAvailable.connect(bind(&BuddycloudBot::handleChannelsServiceInfo, this, _1));
    my_channel_->onReady.connect(bind(&BuddycloudBot::handleOwnChannel, this, _1));
    // Get roster after channels service is found
    my_channel_->onChannelsServiceAvailable.connect(bind(&BuddycloudBot::GetRoster, this));
    // Handle errors
    my_channel_->onError.connect(bind(&BuddycloudBot::handleOwnChannelErrors, this, _1));
    my_channel_->Start();
    channel_controllers_.insert(my_channel_);
  }

  connected_ = true;

}

void BuddycloudBot::handleDisconnected(boost::optional<ClientError> error) {
  if (error) {
    LOG(DEBUG) << "Disconnected with error number " << error.get().getType() << ".";
  }
  connected_ = false;
}

void BuddycloudBot::handleDataRecieved(const SafeByteArray &byte_array) {
  /* WARNING! This breaks the safety of the data in the safe byte array.
     * Do not use in modes that require data safety. */
  //LOG(DEBUG) << safeByteArrayToString(byte_array));
}

void BuddycloudBot::handleIQRecieved(boost::shared_ptr<IQ> iq) {
  //shared_ptr<Body> payload = iq->getPayload<Body>();
  //LOG(DEBUG2) << "IQ recieved"; // << payload->getText();
  /*foreach (const shared_ptr<Payload> p, iq->getPayloads()) {
    LOG(DEBUG) << typeid(*p.get()).name();
  }*/
}

void BuddycloudBot::handleMessageReceived(Message::ref message) {

  LOG(DEBUG2) << "Handling recieved message..";

  if (EventPayload::ref event = message->getPayload<EventPayload>()) {
    LOG(DEBUG) << "event posts node: " << event->getNode();
    BOOST_FOREACH (ChannelController* ch, channel_controllers_) {
      LOG(DEBUG) << "channel posts node: " << ch->posts_node_;
      if (ch->posts_node_ == event->getNode()) {
        LOG(DEBUG) << "ADDING POST TO CHANNEL " << ch->posts_node_;
        BOOST_FOREACH (Atom::ref atom, event->getItems()->get()) {
          Post1::Ref post = TranslateAtom(atom);
          LOG(DEBUG) << "...right now";
          ch->channel_->AddPost(post);
        }
        break;
      }
    }
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
  } else if (presence->getType() == Presence::Subscribed) {

    /*if (BcContact::Ref contact = GetContact(presence->getFrom())) {

    } else {
      BcContact::Ref newc(new BcContact);
      newc->SetUid(presence->getFrom());

      contacts_.push_back(newc);

    }*/

  } else if (presence->getType() == Presence::Unsubscribe) {

  } else if (presence->getType() == Presence::Unsubscribed) {

  }
}

void BuddycloudBot::GetRoster() {
  GetRosterRequest::ref roster = GetRosterRequest::create(client_->getIQRouter());
  roster->onResponse.connect(bind(&BuddycloudBot::handleRosterReceived, this, _1, _2));
  roster->send();
}

void BuddycloudBot::handleRosterReceived(RosterPayload::ref payload, ErrorPayload::ref error) {
  if (error) {
    std::cerr << "Error receiving roster. Continuing anyway.";
  }
  // Send initial available presence
  //client_->sendPresence(Presence::create("Send me a message"));

  BOOST_FOREACH (const RosterItemPayload &item, payload->getItems()) {

    BcContact::Ref contact(new BcContact);
    contact->SetBot(this);
    contact->SetUid(item.getJID());
    contact->SetName(item.getName());
    contact->SetGroups(item.getGroups());
    contact->SetViewable(true);
    contacts_.push_back(contact);

    // Channels service roster item should not be seen in contacts panel
    if (contact->GetUid() == channel_service_.jid.toString())
      contact->SetViewable(false);

    core()->PushContent(this, contact);

  }

  BOOST_FOREACH (const RosterItemPayload &item, payload->getItems()) {
    GetVCardRequest::ref vcard = GetVCardRequest::create(item.getJID(), client_->getIQRouter());
    vcard->send();
  }
}

void BuddycloudBot::AddParserFactories() {
  // Payload parsers and serializers
  AddParserFactory(new GenericPayloadParserFactory<PubsubParser>("pubsub", "http://jabber.org/protocol/pubsub"));
  AddParserFactory(new GenericPayloadParserFactory<EventPayloadParser>("event", "http://jabber.org/protocol/pubsub#event"));
}

void BuddycloudBot::AddSerializers() {
  AddSerializer(new PubsubItemsRequestSerializer);
  AddSerializer(new PubsubRetractRequestSerializer);
  AddSerializer(new PubsubPublishRequestSerializer);
  AddSerializer(new PubsubSubscribeRequestSerializer);
}

void BuddycloudBot::AddParserFactory(Swift::PayloadParserFactory* factory) {
  parsers_.push_back(factory);
  client_->addPayloadParserFactory(factory);
}

void BuddycloudBot::AddSerializer(Swift::PayloadSerializer* serializer) {
  serializers_.push_back(serializer);
  client_->addPayloadSerializer(serializer);
}

/*
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
        this->UserChannelDiscovery(channel_service_.jid, posts_node_);
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

void BuddycloudBot::UserChannelDiscovery(const JID &service_jid, const string &posts_node) {
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(service_jid, posts_node, client_->getIQRouter());
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
        reg->onResponse.connect(bind(&BuddycloudBot::handleOwnChannelRegistration, this, _1, _2));
        reg->send();
      } else {
        LOG(ERROR) << "Registration to channel service '" << channel_service_.jid.toString() << "' is not available!";
      }
    }
  } else {

    BOOST_FOREACH (Form::ref form, payload->getExtensions()) {
      /*LOG(DEBUG) << "Node info form";
      LOG(DEBUG) << form->getField("pubsub#title")->getDescription();
      LOG(DEBUG) << form->getField("pubsub#title")->getLabel();
      LOG(DEBUG) << form->getField("pubsub#title")->getName();
      BOOST_FOREACH (string str, form->getField("pubsub#title")->getRawValues()) {
        LOG(DEBUG2) << "Values:";
        LOG(DEBUG2) << str;
      }*/
    }

    GetPubsubItemsRequest::ref items = GetPubsubItemsRequest::create(channel_service_.jid, posts_node_, client_->getIQRouter());
    items->onResponse.connect(bind(&BuddycloudBot::handleNodeItems, this, _1, _2));
    items->send();
  }
}

void BuddycloudBot::handleOwnChannelRegistration(Payload::ref payload, ErrorPayload::ref error) {

  if (!error) {
    LOG(INFO) << "Channel has registered succesfully.";
  }

  my_channel_->GetChannelContent();

}

void BuddycloudBot::handleNodeItems(PubsubItemsRequest::ref items, ErrorPayload::ref error) {
  LOG(DEBUG) << "handling node items";
}

void BuddycloudBot::handleContactChannel(ChannelController* controller) {
  core()->PushContent(this, controller->channel_);
}

void BuddycloudBot::handleOwnChannel(ChannelController* controller) {
  channel_service_ = controller->service_;
  core()->PushContent(this, controller->channel_);
}

void BuddycloudBot::handleOwnChannelErrors(ChannelController::Error error) {
  switch (error) {
    case ChannelController::UserChannelNotPresent:
      LOG(WARNING) << "Jabber user '" << jid_ << "' has not registered channel yet. \n";
      if (channel_service_.is_registration_available) {
        LOG(TRACE) << "Trying to register user to service '" << channel_service_.jid.toString() << "'.";
        InBandRegistrationPayload::ref payload(new InBandRegistrationPayload);
        //payload->set... // TODO: Set user account data accordingly
        SetInBandRegistrationRequest::ref reg = SetInBandRegistrationRequest::create(channel_service_.jid, payload, client_->getIQRouter());
        reg->onResponse.connect(bind(&BuddycloudBot::handleOwnChannelRegistration, this, _1, _2));
        reg->send();
      } else {
        LOG(ERROR) << "Registration to channel service '" << channel_service_.jid.toString() << "' is not available!";
      }
      break;
    case ChannelController::ChannelsServiceUnavailable:
      break;
    default:
      assert(false);
  }
}

void BuddycloudBot::handleChannelsServiceInfo(ChannelServiceInfo info) {
  channel_service_ = info;
}

/*
 * Channel controller
 */
ChannelController::ChannelController(Contact::Ref contact, BuddycloudBot* bot)
    : channel_(new Channel),
      contact_jid_(contact->GetUid()),
      router_(bot->client_->getIQRouter()),
      bot_(bot) {
  contact_ = contact;
  channel_->SetController(this);
  posts_node_ = "/user/" + contact_jid_.toString() + "/posts";
}

void ChannelController::Start() {
  onChannelAvailable.connect(bind(&ChannelController::GetNodeItems, this, posts_node_));

  GetDiscoItemsRequest::ref items = GetDiscoItemsRequest::create(contact_jid_.getDomain(), router_);
  items->onResponse.connect(bind(&ChannelController::handleDomainItems, this, _1, _2));
  items->send();
}

void ChannelController::Send(Post1::Ref post) {
  PubsubPublishRequest::ref publish(new PubsubPublishRequest);
  Atom::ref atom(new Atom);
  atom->setAuthor(bot_->jid_.toString());
  atom->setContent(post->GetContent());
  atom->setObjectType(Atom::NOTE);
  atom->setVerb(Atom::POST);
  publish->setAtom(atom);
  publish->setNode(posts_node_);
  SetPubsubPublishRequest::ref rq = SetPubsubPublishRequest::create(publish, bot_->channel_service_.jid, router_);
  rq->onResponse.connect(bind(&ChannelController::handleSend, this, post, _1, _2));
  rq->send();
}

void ChannelController::Delete(Post1::Ref post) {
  SetPubsubRetractRequest::ref retract = SetPubsubRetractRequest::create(post->GetID(), posts_node_, bot_->channel_service_.jid, router_);
  retract->onResponse.connect(bind(&ChannelController::handleDelete, this, post, _1, _2));
  retract->send();
}

void ChannelController::handleSend(Post1::Ref post, PubsubPublishRequest::ref publish, Swift::ErrorPayload::ref error) {
  if (error) {
    LOG(ERROR) << error->getText();
    return;
  }
  LOG(DEBUG) << "Post content response with ID: " << publish->getItemID();
  post->SetID(publish->getItemID());
  post->SetAuthor(bot_->jid_.toString());
  channel_->AddPost(post);
}

void ChannelController::handleDelete(Post1::Ref post, PubsubRetractRequest::ref retract, ErrorPayload::ref error) {
  if (error) {
    LOG(ERROR) << error->getText();
    return;
  }
  channel_->RemovePost(post); // TODO: check post's retracted id before removal
}

void ChannelController::handleDomainItems(shared_ptr<DiscoItems> payload, ErrorPayload::ref error) {
  if (error) {
    LOG(ERROR) << error->getText();
    return;
  }
  LOG(TRACE) << "Handling domain disco items ... ";
  vector<DiscoItems::Item>::const_iterator it = payload->getItems().begin();
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), router_);
  info->onResponse.connect(bind(&ChannelController::handleDomainItemInfo, this, _1, _2, payload, it));
  info->send();
}

void ChannelController::handleDomainItemInfo(DiscoInfo::ref payload, ErrorPayload::ref error, shared_ptr<DiscoItems> items, vector<DiscoItems::Item>::const_iterator it) {
  if (error) {
    LOG(ERROR) << error->getText();
  } else {
    BOOST_FOREACH (const DiscoInfo::Identity &identity, payload->getIdentities()) {
      if (identity.getCategory() == "pubsub" && identity.getType() == "channels") {
        service_.is_available = true;
        service_.jid = it->getJID();
        if (payload->hasFeature("jabber:iq:register")) {
          service_.is_registration_available = true;
        } else {
          service_.is_registration_available = false;
        }

        onChannelsServiceAvailable(service_);

        // Discover contacts channel
        GetDiscoInfoRequest::ref info =
            GetDiscoInfoRequest::create(
                service_.jid,
                posts_node_,
                router_);
        info->onResponse.connect(bind(&ChannelController::handleUserChannelDiscovery, this, _1, _2));
        info->send();
      }
    }
  }
  if (!service_.is_available) { // Go for next item if service is not found yet
    ++it;
    if (it == items->getItems().end()) {
      LOG(TRACE) << "Channel service of domain " << contact_jid_.getDomain() << " is not available!";
      onError(ChannelsServiceUnavailable);
    } else {
      GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), router_);
      info->onResponse.connect(bind(&ChannelController::handleDomainItemInfo, this, _1, _2, items, it));
      info->send();
    }
  }
}

void ChannelController::handleUserChannelDiscovery(DiscoInfo::ref payload, ErrorPayload::ref error) {
  if (error) {
    if (error->getCondition() == ErrorPayload::ItemNotFound) {
      onError(UserChannelNotPresent);
    }
  } else {
    BOOST_FOREACH (Form::ref form, payload->getExtensions()) {
      /*LOG(DEBUG) << "Node info form";
      LOG(DEBUG) << form->getField("pubsub#title")->getDescription();
      LOG(DEBUG) << form->getField("pubsub#title")->getLabel();
      LOG(DEBUG) << form->getField("pubsub#title")->getName();
      BOOST_FOREACH (string str, form->getField("pubsub#title")->getRawValues()) {
        LOG(DEBUG2) << "Values:";
        LOG(DEBUG2) << str;
      }*/
    }
    channel_->SetTitle(payload->getExtensions().front()->getField("pubsub#title")->getRawValues().front());
    channel_->SetDescription(payload->getExtensions().front()->getField("pubsub#description")->getRawValues().front());

    onChannelAvailable();

    PubsubSubscribeRequest::ref subscribe(new PubsubSubscribeRequest);
    subscribe->setNode(posts_node_);
    subscribe->setSubscribersJID(bot_->jid_); // purely decorative, just following standard XEP-0060
    SetPubsubSubscribeRequest::ref rq = SetPubsubSubscribeRequest::create(subscribe, service_.jid, router_);
    rq->send();


  }
}

void ChannelController::GetNodeItems(const std::string &node) {
  GetPubsubItemsRequest::ref items = GetPubsubItemsRequest::create(service_.jid, node, router_);
  items->onResponse.connect(bind(&ChannelController::handleNodeItems, this, _1, _2));
  items->send();
}

void ChannelController::handleNodeItems(PubsubItemsRequest::ref items, ErrorPayload::ref error) {
  if (error) {
    LOG(ERROR) << error->getText();
    return;
  }
  BOOST_FOREACH (const Atom::ref &atom, items->getItems()->get()) {
    Post1::Ref post(new Post1);
    post->SetID(atom->getID());
    post->SetAuthor(atom->getAuthor());
    post->SetContent(atom->getContent());
    channel_->AddPost(post);
  }
  onReady(this);
}
