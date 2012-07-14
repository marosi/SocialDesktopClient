/*
 * buddycloud_bot.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 28, 2012
 */

#include "bc_model.h"
#include "bc_contact.h"
#include "channel.h"
#include "post.h"
#include "payloads/pubsub.h"
#include "pubsub_requests.h"
#include "pubsub_parser.h"
#include "pubsub_serializer.h"
#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"
#include <fstream>

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
      own_channel_(0),
      jid_(account->GetUid()),
      posts_node_("/user/" + jid_.toString() + "/posts"),
      connected_(false) {
  LOG(DEBUG) << "This is buddycloud Service Model!";
  //Swift::logging = true;
  avatar_file_path_ = ""; // TODO: implement default avatar
  /*
   * Swiften setup
   */
  loop_ = new SimpleEventLoop;
  network_ = new BoostNetworkFactories(loop_);
  //client_ = new Client("sdc_test@jabbim.com", "sdc_test", network_);
  //client_ = new Client("maros@buddycloud.org", "udsampia", network_);
  //client_ = new Client("pista@localhost", "pista", network_);
  //client_ = new Client("test_subject@buddycloud.org", "test", network_);
  client_ = new Client(jid_, account->GetPassword(), network_);

  client_->setAlwaysTrustCertificates();
  client_->onConnected.connect(bind(&BcModel::handleConnected, this));
  client_->onDisconnected.connect(bind(&BcModel::handleDisconnected, this, _1));
  client_->onMessageReceived.connect(bind(&BcModel::handleMessageReceived, this, _1));
  client_->onPresenceReceived.connect(bind(&BcModel::handlePresenceReceived, this, _1));
  //client_->onDataRead.connect(bind(&BcModel::handleDataRecieved, this, _1));
  //client_->getStanzaChannel()->onIQReceived.connect(bind(&BcModel::handleIQRecieved, this, _1));
  /*
   * Roster actions
   */
  client_->getRoster()->onJIDAdded.connect(bind(&BcModel::handleJidAdded, this, _1));
  client_->getRoster()->onJIDRemoved.connect(bind(&BcModel::handleJidRemove, this, _1));
  client_->getRoster()->onJIDUpdated.connect(bind(&BcModel::handleJidUpdated, this, _1, _2, _3));
  /*
   * VCard actions
   */
  client_->getVCardManager()->onOwnVCardChanged.connect([&] (VCard::ref vcard) {
    if (vcard->getPhoto().size() > 0) {
      const std::string file_path = SavePhoto(jid_, vcard);
      if (file_path != "") {
        this->account()->SetProperty("avatars/own", file_path);
        onOwnAvatarChange(file_path);
      }
    }
  });
  client_->getVCardManager()->onVCardChanged.connect([&] (const JID &jid, VCard::ref vcard) {
    if (vcard->getPhoto().size() > 0) {
      const std::string file_path = SavePhoto(jid, vcard);
      if (file_path != "") {
        this->account()->SetProperty("avatars/" + jid.toString(), file_path);
        GetContact(jid)->onAvatarChanged();
      }
    }
  });
  /*
   * Subscriptions
   */
  client_->getSubscriptionManager()->onPresenceSubscriptionRequest.connect([&] (const JID &jid, const string &str, Presence::ref presence) {
    LOG(DEBUG) << "Subscription request from " << jid.toString() << " and message?: " << str;
  });

  client_->getSubscriptionManager()->onPresenceSubscriptionRevoked.connect([&] (const JID &jid, const string &str) {
    LOG(DEBUG) << "Subscription of " << jid.toString() << " has been revoked: " << str;
  });
  // Tracers
  tracer_ = new ClientXMLTracer(client_);
  // Add parsers and serializers
  AddParserFactories();
  AddSerializers();
}

BcModel::~BcModel() {
  client_->disconnect();
  LOG(TRACE) << "Buddycloud service model is destroying";
  if (own_channel_)
    delete own_channel_;
  for (ChannelController* ch : channels_) {
    delete ch;
  }
  for (BcContact* c : contacts_) {
    delete c;
  }
  for (PayloadParserFactory* pf : parsers_) {
    delete pf;
  }
  for (PayloadSerializer* ps : serializers_) {
    delete ps;
  }
  delete tracer_;
  delete client_;
}

void BcModel::AddNewContact(const JID &jid) {
  if (contacts_map_.count(jid) == 0) {
//    RosterItemPayload item;
//    item.setJID(contact->GetUid());
//    item.setSubscription(RosterItemPayload::To);
//    item.setSubscriptionRequested();
//    RosterPayload::ref roster(new RosterPayload);
//    roster->addItem(item);
//    SetRosterRequest::ref set = SetRosterRequest::create(roster, client_->getIQRouter());
//    set->send();
    client_->getSubscriptionManager()->requestSubscription(jid);
  }
}

void BcModel::RemoveContact(const JID &jid) {
  /*Presence::ref presence(new Presence);
  presence->setTo(contact->GetUid());
  presence->setType(Presence::Unsubscribe);
  client_->sendPresence(presence);*/

  RosterPayload::ref roster(new RosterPayload);
  RosterItemPayload item;
  item.setJID(jid);
  item.setSubscription(RosterItemPayload::Remove);
  roster->addItem(item);
  SetRosterRequest::ref set = SetRosterRequest::create(roster, client_->getIQRouter());
  set->send();
}

BcContact* BcModel::GetContact(const Swift::JID &jid) {
  vector<BcContact*>::iterator it =
      std::find_if(contacts_.begin(), contacts_.end(),
                   [&] (const BcContact* contact) { return contact->GetUid() == jid.toString(); });
  if (it != contacts_.end())
    return *it;
  else
    return 0;
}

ChannelController* BcModel::GetChannel(const Swift::JID &jid) {
  if (channels_map_.count(jid) > 0)
    return channels_map_[jid];
  ChannelController* channel = new ChannelController(jid, this);
  channels_.push_back(channel);
  channels_map_[jid] = channel;
  return channel;
}

void BcModel::GetServerInfo() {
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(jid_.getDomain(), client_->getIQRouter());
  info->onResponse.connect([&] (DiscoInfo::ref payload, ErrorPayload::ref error) {
    if (!error)
      server_info_ = payload;
  });
  info->send();
}

void BcModel::test() {
  //client_->getVCardManager()->requestOwnVCard();
}

/*
 * Connected/disconnected handlers
 */

void BcModel::handleConnected() {
  LOG(TRACE) << "XMPP client is connected" << std::endl;

  // request own vCard


  client_->getRoster()->onInitialRosterPopulated.connect([&] () {
    LOG(TRACE) << "Requesting vCards for contacts";
    // request vCards
    for (Contact* c : contacts_) {
      client_->getVCardManager()->requestVCard(c->GetUid());
    }
  });
  client_->requestRoster();



  // Send presence message
  client_->sendPresence(Presence::create("Ready when you are..."));
  // initialize own channel
//  if (!own_channel_) {
//    own_channel_ = new ChannelController(jid_, this);
//    own_channel_->onChannelsServiceAvailable.connect([&] (ChannelServiceInfo info) {
//      service_jid_ = info.jid;
//      is_service_registration_available_ = info.is_registration_available;
//    });
//    // Get roster after channels service is found
//    //my_channel_->onChannelsServiceAvailable.connect(bind(&BcModel::GetRoster, this));
//    // Handle errors
//    own_channel_->onError.connect([&] (ChannelController::Error error) {
//      switch (error) {
//        case ChannelController::UserChannelNotPresent:
//          LOG(WARNING) << "Jabber user '" << jid_ << "' has not registered channel yet. \n";
//          if (is_service_registration_available_) {
//            LOG(TRACE) << "Trying to register user to service '" << service_jid_.toString() << "'.";
//            InBandRegistrationPayload::ref payload(new InBandRegistrationPayload);
//            //payload->set... // TODO: Set user account data accordingly
//            SetInBandRegistrationRequest::ref reg = SetInBandRegistrationRequest::create(service_jid_, payload, client_->getIQRouter());
//            reg->onResponse.connect([&] (Payload::ref payload, ErrorPayload::ref error) {
//              /*
//               * Second level asynchronous call
//               */
//              if (!error) {
//                LOG(INFO) << "Channel has registered succesfully.";
//                onSelfChannelRegistered();
//              }
//            });
//            reg->send();
//          } else {
//            LOG(ERROR) << "Registration to channel service '" << service_jid_.toString() << "' is not available!";
//          }
//          break;
//        case ChannelController::ChannelsServiceUnavailable:
//          break;
//        default:
//          assert(false);
//          break;
//      }
//    });
//    own_channel_->DiscoverService();
//  }
  connected_ = true;
  onConnected();

}

void BcModel::handleDisconnected(boost::optional<ClientError> error) {
  if (error) {
    LOG(DEBUG) << "Disconnected with error number " << error.get().getType() << ".";
  }
  connected_ = false;
}

/*
 * Permanent stanza handling
 */
void BcModel::handleMessageReceived(Message::ref message) {
  LOG(DEBUG) << "Handling recieved message from: " << message->getFrom();
  // is message event payload
  if (EventPayload::ref event = message->getPayload<EventPayload>()) {
    LOG(DEBUG2) << "Event from node: " << event->getNode();
    for (ChannelController* channel : channels_) {
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

  } else if (presence->getType() == Presence::Unsubscribe) {

  } else if (presence->getType() == Presence::Unsubscribed) {

  }
}

/*
 * Roster change handlers
 */

void BcModel::handleJidAdded(const JID &jid) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been added to roster";
  XMPPRosterItem item = client_->getRoster()->getItem(jid).get();
  switch (item.getSubscription()) {
    case RosterItemPayload::None:
      break;
    case RosterItemPayload::Remove:
      break;
    case RosterItemPayload::From:
      break;
    case RosterItemPayload::To:
      AddContact(item);
      break;
    case RosterItemPayload::Both:
      AddContact(item);
      break;
    default:
      break;
  }
}

void BcModel::handleJidRemove(const JID &jid) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been removed from the roster";
  vector<BcContact*>::iterator it =
      std::find_if(contacts_.begin(), contacts_.end(),
                   [&] (const BcContact* contact) { return contact->GetUid() == jid.toString(); });
  if (it != contacts_.end()) {
    contacts_.erase(it);
    contacts_map_.erase(jid);
  }
}

void BcModel::handleJidUpdated(const JID &jid, const string &name, const vector<string> &groups) {
  LOG(INFO) << "JID '" << jid.toString() << "' has been updated in roster";
  XMPPRosterItem item = client_->getRoster()->getItem(jid).get();
  switch (item.getSubscription()) {
    case RosterItemPayload::None:
      break;
    case RosterItemPayload::Remove:
      break;
    case RosterItemPayload::From:
      break;
    case RosterItemPayload::To:
      AddContact(item);
      break;
    case RosterItemPayload::Both:
      AddContact(item);
      break;
    default:
      break;
  }
}

/*
 *
 */

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
  SetPubsubConfigureNodeRequest::ref conf = SetPubsubConfigureNodeRequest::create(payload, service_jid_, client_->getIQRouter());
  conf->send();
}

void BcModel::Unsubscribe() {

  //GetRoster();

  PubsubUnsubscribeRequest::ref payload(new PubsubUnsubscribeRequest);
  payload->setNode("/user/harry@channel.r.gd/posts");
  SetPubsubUnsubscribeRequest::ref uns = SetPubsubUnsubscribeRequest::create(payload, service_jid_, client_->getIQRouter());
  uns->send();
}

/*
 * Parsers and serializers
 */

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

void BcModel::AddContact(const Swift::XMPPRosterItem &item) {
  if (contacts_map_.count(item.getJID()) == 0) {
    LOG(TRACE) << "Adding contact " << item.getJID().toString() << " to contact list.";
    BcContact* contact = new BcContact(this, item.getJID());
    contact->SetUid(item.getJID());
    contact->SetName(item.getName());
    contact->SetGroups(item.getGroups());
    // add to member structures
    contacts_.push_back(contact);
    contacts_map_[item.getJID()] = contact;
    onContactAdded(item.getJID());
  }
}

/*
 * Miscellaneous
 */
const std::string BcModel::SavePhoto(const Swift::JID &jid, Swift::VCard::ref vcard) {
  string ext = "";
  if (vcard->getPhotoType() == "image/png")
    ext = "png";
  else if (vcard->getPhotoType() == "image/jpeg")
    ext = "jpeg";
  else if (vcard->getPhotoType() == "image/gif")
    ext = "gif";
  if (ext != "") {
    std::ofstream file;
    std::string avatar_file = this->account()->GetDir() + "/" + jid.toString() + "." + ext;
    file.open(avatar_file);
    file << byteArrayToString(vcard->getPhoto());
    file.close();
    return avatar_file;
  }
  // something went wrong
  return "";
}