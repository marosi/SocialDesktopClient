/*
 * buddycloud_bot.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 28, 2012
 */

#include "bc_model.h"
#include "channel.h"
#include "pubsub_requests.h"
#include "pubsub_parser.h"
#include "pubsub_serializer.h"
#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"

using namespace Swift;
using namespace boost;
using namespace sdc;
using std::string;
using std::vector;

// TRYING TO REGISTER USER BY INBANDREGISTRATION RIGHT AFTER STREAM INITIALIZATION
// CAN'T BE DONE WITH Swift::Client
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
BcModel::BcModel(sdc::AccountData* account)
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
      bind(&BcModel::handleConnected, this));
  client_->onDisconnected.connect(
      bind(&BcModel::handleDisconnected, this, _1));
  client_->onMessageReceived.connect(
      bind(&BcModel::handleMessageReceived, this, _1));
  client_->onPresenceReceived.connect(
      bind(&BcModel::handlePresenceReceived, this, _1));
  client_->onDataRead.connect( // TODO :: remove
      bind(&BcModel::handleDataRecieved, this, _1));
  client_->getStanzaChannel()->onIQReceived.connect(
      bind(&BcModel::handleIQRecieved, this, _1));

  client_->getRoster()->onJIDAdded.connect(bind(&BcModel::handleJidAdded, this, _1));
  client_->getRoster()->onJIDRemoved.connect(bind(&BcModel::handleJidRemove, this, _1));
  client_->getRoster()->onJIDUpdated.connect(bind(&BcModel::handleJidUpdated, this, _1, _2, _3));

  // Tracer
  tracer_ = new ClientXMLTracer(client_);
  // Add parsers and serializers
  AddParserFactories();
  AddSerializers();
}

ChannelController* BcModel::GetChannel(const Swift::JID &jid) {
  if (channels_.count(jid) > 0)
    return channels_[jid];
  ChannelController* channel = new ChannelController(jid, this);
  channel_controllers_.insert(channel);
  return channel;
}

void BcModel::AddNewContact(BcContact::Ref contact) {
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

void BcModel::RemoveContact(BcContact::Ref contact) {
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

void BcModel::handleJidAdded(const JID &jid) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been added to roster";
  XMPPRosterItem item = client_->getRoster()->getItem(jid).get();
  BcContact::Ref contact(new BcContact(this, jid));
  contact->SetUid(item.getJID());
  contact->SetName(item.getName());
  contact->SetGroups(item.getGroups());
  contacts_.push_back(contact);
  //core()->PushContent(this, contact);
  GetChannel(contact->GetUid());
}

void BcModel::handleJidRemove(const JID &jid) {
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
    //core()->RemoveContent(*it);
    contacts_.erase(it);
  }
}

void BcModel::handleJidUpdated(const JID &jid, const string &name, const vector<string> &groups) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been updated in roster";
}

BcModel::~BcModel() {
  client_->disconnect();

  LOG(TRACE) << "Buddycloud service model is destroying";
  for (BcContact::Ref &contact : contacts_) {
    //core()->RemoveContent(contact);
  }

  if (my_channel_)
    delete my_channel_;

  // TODO: change this to new standard for and TEST it
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

void BcModel::SendDiscoInfo(const string &to_attribute, const string &node_attribute) {
  JID jid(to_attribute);
  GetDiscoInfoRequest::ref di = GetDiscoInfoRequest::create(jid, node_attribute, client_->getIQRouter());
  di->send();
}

void BcModel::SendDiscoItems(const string &to_attribute, const string &node_attribute) {
  JID jid(to_attribute);
  GetDiscoItemsRequest::ref di = GetDiscoItemsRequest::create(jid, node_attribute, client_->getIQRouter());
  di->send();
}

BcContact::Ref BcModel::GetContact(const Swift::JID &jid) {
  for (BcContact::Ref &c : contacts_) {
    if (c->GetUid() == jid.toString())
      return c;
  }
  return BcContact::Ref();
}

// TODO: delete probably
/*Post1::Ref BcModel::TranslateAtom(const Atom::ref atom) {
  Post1* post = new Post1();
  post->SetID(atom->getID());
  post->SetAuthor(atom->getAuthor());
  post->SetContent(atom->getContent());
  return post;
}*/

void BcModel::GetServerInfo() {
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(jid_.getDomain(), client_->getIQRouter());
  info->onResponse.connect([&] (DiscoInfo::ref payload, ErrorPayload::ref error) {
    if (!error)
      server_info_ = payload;
  });
  info->send();
}

void BcModel::handleConnected() {
  LOG(TRACE) << "XMPP client is connected" << std::endl;
  // Send presence message
  client_->sendPresence(Presence::create("Ready when you are..."));

  if (!my_channel_) {
    my_channel_ = new ChannelController(jid_, this);
    my_channel_->onChannelsServiceAvailable.connect([&] (ChannelServiceInfo info) {
      channel_service_ = info;
    });
    /*my_channel_->onReady.connect([&] (ChannelController* controller) {
      channel_service_ = controller->service_;
      core()->PushContent(this, controller->channel_);
    });*/
    // Get roster after channels service is found
    my_channel_->onChannelsServiceAvailable.connect(bind(&BcModel::GetRoster, this));
    // Handle errors
    my_channel_->onError.connect([&] (ChannelController::Error error) {
      switch (error) {
        case ChannelController::UserChannelNotPresent:
          LOG(WARNING) << "Jabber user '" << jid_ << "' has not registered channel yet. \n";
          if (channel_service_.is_registration_available) {
            LOG(TRACE) << "Trying to register user to service '" << channel_service_.jid.toString() << "'.";
            InBandRegistrationPayload::ref payload(new InBandRegistrationPayload);
            //payload->set... // TODO: Set user account data accordingly
            SetInBandRegistrationRequest::ref reg = SetInBandRegistrationRequest::create(channel_service_.jid, payload, client_->getIQRouter());
            reg->onResponse.connect([&] (Payload::ref payload, ErrorPayload::ref error) {
              /*
               * Second level asynchronous call
               */
              if (!error) {
                LOG(INFO) << "Channel has registered succesfully.";
                onSelfChannelRegistered();
              }
            });
            reg->send();
          } else {
            LOG(ERROR) << "Registration to channel service '" << channel_service_.jid.toString() << "' is not available!";
          }
          break;
        case ChannelController::ChannelsServiceUnavailable:
          break;
        default:
          assert(false);
          break;
      }
    });
    my_channel_->DiscoverService();
    channel_controllers_.insert(my_channel_);
  }
  LOG(DEBUG) << " In handleConnected thread ID: " << boost::this_thread::get_id();
  connected_ = true;
  onConnected();

}

void BcModel::handleDisconnected(boost::optional<ClientError> error) {
  if (error) {
    LOG(DEBUG) << "Disconnected with error number " << error.get().getType() << ".";
  }
  connected_ = false;
}

void BcModel::handleDataRecieved(const SafeByteArray &byte_array) {
  /* WARNING! This breaks the safety of the data in the safe byte array.
     * Do not use in modes that require data safety. */
  //LOG(DEBUG) << safeByteArrayToString(byte_array));
}

void BcModel::handleIQRecieved(boost::shared_ptr<IQ> iq) {
  //shared_ptr<Body> payload = iq->getPayload<Body>();
  //LOG(DEBUG2) << "IQ recieved"; // << payload->getText();
  /*foreach (const shared_ptr<Payload> p, iq->getPayloads()) {
    LOG(DEBUG) << typeid(*p.get()).name();
  }*/
}

void BcModel::handleMessageReceived(Message::ref message) {

  LOG(DEBUG) << "Handling recieved message from: " << message->getFrom();
  // is message event payload
  if (EventPayload::ref event = message->getPayload<EventPayload>()) {
    LOG(DEBUG2) << "Event from node: " << event->getNode();
    for (ChannelController* channel : channel_controllers_) {
      if (channel->posts_node_ == event->getNode()) {
        for (Atom::ref atom : event->getItems()->get()) {
          if (atom->getInReplyTo() != "") {
            Post1* post = channel->GetPost(atom->getInReplyTo());
            if (post) {
              Comment* comment = new Comment(post, atom->getContent());
              comment->SetAuthor(atom->getAuthor());
              comment->SetID(atom->getID());
              post->AddComment(comment);
            }
          } else {
            Post1* post = new Post1(channel);
            post->SetID(atom->getID());
            post->SetAuthor(atom->getAuthor());
            post->SetContent(atom->getContent());
            channel->AddPost(post);
          }
        }
        break;
      }
    }
  }
}

void BcModel::handlePresenceReceived(Presence::ref presence) {
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

void BcModel::GetRoster() {
  GetRosterRequest::ref roster = GetRosterRequest::create(client_->getIQRouter());
  roster->onResponse.connect([&] (RosterPayload::ref payload, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << "Error receiving roster. Continuing anyway.";
    }
    //  Process roster items
    for (const RosterItemPayload &item : payload->getItems()) {
      BcContact::Ref contact(new BcContact(this, item.getJID()));
      contact->SetUid(item.getJID());
      contact->SetName(item.getName());
      contact->SetGroups(item.getGroups());
      contacts_.push_back(contact);
      // Channels service roster item should not be seen in contacts panel
      //if (contact->GetUid() == channel_service_.jid.toString()) {
      //  contact->SetViewable(false);
      //}
      //core()->PushContent(this, contact);
    }
    // Get VCards
    for (const RosterItemPayload &item : payload->getItems()) {
      GetVCardRequest::ref vcard = GetVCardRequest::create(item.getJID(), client_->getIQRouter());
      vcard->send();
    }
    onContactsReady(contacts_);
  });
  roster->send();
}

void BcModel::ToggleChannelPrivacy() {
  PubsubConfigureNodeRequest::ref payload(new PubsubConfigureNodeRequest);
  payload->setNode("/user/" + jid_.toString());
  Form::ref form(new Form(Form::SubmitType));
  FormField::ref f1 = HiddenFormField::create("http://jabber.org/protocol/pubsub#node_config");
  f1->setName("FORM_TYPE");
  form->addField(f1);

  FormField::ref f2 = TextSingleFormField::create("whitelist");
  f2->setName("pubsub#access_model");
  form->addField(f2);

  /*f2 = FixedFormField::create("whitelist");
  f2->setName("pubsub#access_model");
  form->addField(f2);

  f2 = ListSingleFormField::create("whitelist");
  f2->setName("pubsub#access_model");
  form->addField(f2);

  f2 = HiddenFormField::create("whitelist");
  f2->setName("pubsub#access_model");
  form->addField(f2);

  f2 = TextMultiFormField::create("whitelist");
  f2->setName("pubsub#access_model");
  form->addField(f2);

  f2 = TextPrivateFormField::create("whitelist");
  f2->setName("pubsub#access_model");
  form->addField(f2);*/

  payload->setForm(form);
  SetPubsubConfigureNodeRequest::ref conf = SetPubsubConfigureNodeRequest::create(payload, channel_service_.jid, client_->getIQRouter());
  conf->send();
}

void BcModel::Unsubscribe() {

  GetRoster();

  PubsubUnsubscribeRequest::ref payload(new PubsubUnsubscribeRequest);
  payload->setNode("/user/harry@channel.r.gd/posts");
  SetPubsubUnsubscribeRequest::ref uns = SetPubsubUnsubscribeRequest::create(payload, channel_service_.jid, client_->getIQRouter());
  uns->send();
}

void BcModel::AddParserFactories() {
  // Payload parsers and serializers
  AddParserFactory(new GenericPayloadParserFactory<PubsubParser>("pubsub", "http://jabber.org/protocol/pubsub"));
  AddParserFactory(new GenericPayloadParserFactory<EventPayloadParser>("event", "http://jabber.org/protocol/pubsub#event"));
}

void BcModel::AddSerializers() {
  AddSerializer(new PubsubItemsRequestSerializer);
  AddSerializer(new PubsubRetractRequestSerializer);
  AddSerializer(new PubsubPublishRequestSerializer);
  AddSerializer(new PubsubSubscribeRequestSerializer);
  AddSerializer(new PubsubUnsubscribeRequestSerializer);
  AddSerializer(new PubsubConfigureNodeRequestSerializer);
}

void BcModel::AddParserFactory(Swift::PayloadParserFactory* factory) {
  parsers_.push_back(factory);
  client_->addPayloadParserFactory(factory);
}

void BcModel::AddSerializer(Swift::PayloadSerializer* serializer) {
  serializers_.push_back(serializer);
  client_->addPayloadSerializer(serializer);
}
