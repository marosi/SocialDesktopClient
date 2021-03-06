/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "bc_model.h"
#include "bc_contact.h"
#include "channel.h"
#include "discover_service.h"
#include "filesystem_storages.h"
#include "payloads/pubsub.h"
#include "post.h"
#include "pubsub_requests.h"
#include "pubsub_parser.h"
#include "pubsub_serializer.h"
#include "sdc/core/account.h"
#include "Swiften/Swiften.h"
#include "boost/foreach.hpp"
#include "boost/shared_ptr.hpp"
#include <fstream>

using namespace Swift;
using namespace boost;
using namespace sdc;
using std::string;
using std::vector;

/*
 * PUBLIC INTERFACE
 */
BcModel::BcModel(sdc::Account* account)
    : QtServiceModel(account),
      own_channel_(0) {
  // initialize
  jid_ = account->GetUid();
  LOG(DEBUG) << "This is buddycloud Service Model!";
  //Swift::logging = true;
  /*
   * Swiften setup
   */
  loop_ = new SimpleEventLoop;
  network_ = new BoostNetworkFactories(loop_);
  storages_ = new FilesystemStorages(service()->dir(), account);
  client_ = new Client(jid_, account->GetPassword(), network_, storages_);

  client_->setAlwaysTrustCertificates();
  client_->onConnected.connect(bind(&BcModel::handleConnected, this));
  client_->onDisconnected.connect(bind(&BcModel::handleDisconnected, this, _1));
  client_->onMessageReceived.connect(bind(&BcModel::handleMessageReceived, this, _1));
  client_->onPresenceReceived.connect(bind(&BcModel::handlePresenceReceived, this, _1));

  /*
   * Roster actions
   */
  client_->getRoster()->onJIDAdded.connect(bind(&BcModel::handleJidAdded, this, _1));
  client_->getRoster()->onJIDRemoved.connect(bind(&BcModel::handleJidRemove, this, _1));
  client_->getRoster()->onJIDUpdated.connect(bind(&BcModel::handleJidUpdated, this, _1, _2, _3));

  subscriber_.onAvailable.connect([&] (DiscoverService::Info service, JID jid) {
    SetPubsubSubscribeRequest::ref sub = SetPubsubSubscribeRequest::create(Channel::GetPostsNode(jid), service.jid, client_->getIQRouter());
    sub->onResponse.connect([&] (PubsubSubscribeRequest::ref, ErrorPayload::ref error) {
      if (!error) {
        GetOwnChannel()->RetrieveSubscriptions();
      }
    });
    sub->send();
  });

  unsubscriber_.onAvailable.connect([&] (DiscoverService::Info service, JID jid) {
    SetPubsubUnsubscribeRequest::ref uns = SetPubsubUnsubscribeRequest::create(Channel::GetPostsNode(jid), service.jid, client_->getIQRouter());
    uns->onResponse.connect([&] (PubsubUnsubscribeRequest::ref, ErrorPayload::ref error) {
      if (!error) {
        GetOwnChannel()->RetrieveSubscriptions();
      }
    });
    uns->send();
  });

  /*
   * VCard actions
   */
  /*
  client_->getVCardManager()->onOwnVCardChanged.connect([&] (VCard::ref vcard) {
  });
  client_->getVCardManager()->onVCardChanged.connect([&] (const JID &jid, VCard::ref vcard) {
  });
  */

  /*
   * Avatar management
   */
  client_->getAvatarManager()->onAvatarChanged.connect([&] (const JID &jid) {
    if (jid == jid_) {
      LOG(DEBUG) << "Changin OWN avatar!";
    }
    onAvatarChanged(jid);
    LOG(DEBUG) << "Avatar of " << jid.toString() << " changed";
  });

  /*
   * Presence handling
   */
  client_->getPresenceOracle()->onPresenceChange.connect([&] (Presence::ref presence) {
    JID jid = presence->getFrom().toBare();
    if (contacts_map_.count(jid) > 0) {
      contacts_map_[jid]->SetStatus(GetStatus(presence));
    }
    LOG(DEBUG) << "Presence changed by " << presence->getFrom().toString() << " to " << GetStatus(presence);
  });

  /*
   * Subscriptions
   */
  client_->getSubscriptionManager()->onPresenceSubscriptionRequest.connect([&] (const JID &jid, const string &str, Presence::ref /*presence*/) {
    LOG(DEBUG) << "Subscription request from " << jid.toString() << " and message: " << str;
  });
  client_->getSubscriptionManager()->onPresenceSubscriptionRevoked.connect([&] (const JID &jid, const string &str) {
    LOG(DEBUG) << "Subscription of " << jid.toString() << " has been revoked: " << str;
  });

  // Tracers
  //tracer_ = new ClientXMLTracer(client_);

  // Add parsers and serializers
  AddParserFactories();
  AddSerializers();
}

BcModel::~BcModel() {
  client_->disconnect();
  LOG(TRACE) << "Buddycloud service model is destroying";
  if (own_channel_)
    delete own_channel_;
  BOOST_FOREACH (Channel* ch , channels_) {
    delete ch;
  }
  BOOST_FOREACH (BcContact* c , contacts_) {
    delete c;
  }
  BOOST_FOREACH (PayloadParserFactory* pf , parsers_) {
    delete pf;
  }
  BOOST_FOREACH (PayloadSerializer* ps , serializers_) {
    delete ps;
  }
  //delete tracer_;
  delete client_;
  delete loop_;
  delete network_;
}

void BcModel::Run() {
  LOG(INFO) << "Buddycloud service model is running.";
  loop_->run();
}

void BcModel::Stop() {
  if (client_->isActive())
    client_->disconnect();
  loop_->stop();
  LOG(INFO) << "Buddycloud service model stopped.";
}

void BcModel::Connect() {
  if (!client_->isActive()) {
    client_->connect();
    LOG(TRACE) << "Connecting Swiften XMPP client.";
  }
}

void BcModel::Disconnect() {
  if (client_->isActive()) {
    client_->disconnect();
    LOG(TRACE) << "Disconnecting Swiften XMPP client.";
  }
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

BcContact* BcModel::GetContact(const Swift::JID &jid) {
  vector<BcContact*>::iterator it =
      std::find_if(contacts_.begin(), contacts_.end(),
                   [&] (const BcContact* contact) { return contact->GetUid() == jid.toString(); });
  if (it != contacts_.end())
    return *it;
  else
    return 0;
}

Channel* BcModel::GetChannel(const Swift::JID &jid) {
  if (channels_map_.count(jid) > 0)
    return channels_map_[jid];
  return CreateChannel(jid);
}

void BcModel::GetServerInfo() {
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(jid_.getDomain(), client_->getIQRouter());
  info->onResponse.connect([&] (DiscoInfo::ref payload, ErrorPayload::ref error) {
    if (!error)
      server_info_ = payload;
  });
  info->send();
}

const std::string BcModel::GetOwnAvatarPath() {
  return GetAvatarPath(jid_);
}

const std::string BcModel::GetAvatarPath(const JID &jid) {
  string file = client_->getAvatarManager()->getAvatarPath(jid).file_string();
  if (!file.empty())
    return file;
  else
    return GetDefaultAvatarPath();
}

const std::string BcModel::GetDefaultAvatarPath() {
  return service()->dir() + "/default_avatar.png";
}

/*
 * Connected/disconnected handlers
 */
void BcModel::handleConnected() {
  LOG(TRACE) << "XMPP client is connected" << std::endl;
  // request contact vCards and create channel for each contact
  client_->getRoster()->onInitialRosterPopulated.connect([&] () {
    LOG(TRACE) << "Requesting vCards for contacts";
    BOOST_FOREACH (Contact* c , contacts_) {
      client_->getVCardManager()->requestVCard(c->GetUid());
      CreateChannel(c->GetUid());
    }
  });
  client_->requestRoster();
  // request own vCard
  client_->getVCardManager()->requestOwnVCard();
  // initialize own channel
  if (!own_channel_) {
    own_channel_ = CreateChannel(jid_);
    own_channel_->onChannelsServiceAvailable.connect([&] (Channel::ChannelServiceInfo info) {
      service_jid_ = info.jid;
      is_service_registration_available_ = info.is_registration_available;
    });
    // Handle errors
    own_channel_->onError.connect([&] (Channel::Error error) {
      switch (error) {
        case Channel::UserChannelNotPresent:
          LOG(WARNING) << "Jabber user '" << jid_ << "' has not registered channel yet. \n";
          if (is_service_registration_available_) {
            LOG(TRACE) << "Trying to register user to service '" << service_jid_.toString() << "'.";
            InBandRegistrationPayload::ref payload(new InBandRegistrationPayload);
            //payload->set... // TODO: Set user account data accordingly
            SetInBandRegistrationRequest::ref reg = SetInBandRegistrationRequest::create(service_jid_, payload, client_->getIQRouter());
            reg->onResponse.connect([&] (Payload::ref /*payload*/, ErrorPayload::ref error) {
              /*
               * Second level asynchronous call
               */
              if (!error) {
                LOG(INFO) << "Channel has registered succesfully.";
                onSelfChannelRegistered();
                own_channel_->Sync();
              }
            });
            reg->send();
          } else {
            LOG(ERROR) << "Registration to channel service '" << service_jid_.toString() << "' is not available!";
          }
          break;
        case Channel::ChannelsServiceUnavailable:
          onError(ServiceUnavailable);
          LOG(ERROR) << "Channels service unavailable.";
          break;
        default:
          assert(false);
          break;
      }
    });
    own_channel_->onChannelAvailable.connect([&] () {
      onChannelAvailable();
      own_channel_->RetrieveSubscriptions();
    });
  }
  onConnected();
  client_->sendPresence(Presence::create("I'm here"));
}

void BcModel::handleDisconnected(boost::optional<ClientError> error) {
  if (error) {
    LOG(DEBUG) << "Disconnected with error number " << error.get().getType() << ".";
  }
  onDisconnected();
}

/*
 * Permanent stanza handling
 */
void BcModel::handleMessageReceived(Message::ref message) {
  LOG(DEBUG) << "Handling recieved message from: " << message->getFrom();
  // is message event payload
  if (EventPayload::ref event = message->getPayload<EventPayload>()) {
    LOG(DEBUG2) << "Event from node: " << event->getNode();
    vector<shared_ptr<DiscoItems> > dits = event->getItems()->getInternal<DiscoItems>();
    BOOST_FOREACH (boost::shared_ptr<DiscoItems> discoitems, dits) {
      BOOST_FOREACH (DiscoItems::Item item, discoitems->getItems()) {
        LOG(DEBUG) << "Subscription to node " <<  item.getNode() << " added to subscripstions storage.";
        //GetOwnChannel()->RetrieveSubscriptions();
      }
    }
    // take all atoms
    vector<Atom::ref> atoms = event->getItems()->getInternal<Atom>();

    // extract JID from node ID
    string node = event->getNode();
    size_t pos = node.find("/posts");
    string node_jid;
    if (pos != string::npos) {
      node_jid = node.substr(6, pos - 6);
    }

    BOOST_FOREACH (Atom::ref at, atoms) {
      Activity act;
      act.from = at->getAuthorJID();
      act.to = node_jid;
      if (at->getObjectType() == Atom::Note) {
        act.verb = Activity::Note;
      } else if (at->getObjectType() == Atom::Comment) {
        act.verb = Activity::Comment;
      }
      onNewActivity(act);
    }

    BOOST_FOREACH (Channel* channel , channels_) {
      if (channel->posts_node_ == event->getNode()) {
        BOOST_FOREACH (Atom::ref atom , atoms) {
          if (atom->getObjectType() == Atom::Comment) {
            LOG(DEBUG4) << "In reply to: " << atom->getInReplyTo();
            Post* post = channel->GetPost(atom->getInReplyTo());
            if (post) {
              post->AddComment(atom);
            }
          } else {
            channel->AddPost(atom);
          }
        }
      }
    }
  }
}

void BcModel::handlePresenceReceived(Presence::ref presence) {
  // Automatically approve subscription requests
  if (presence->getType() == Presence::Subscribe) {
    LOG(INFO) << "Presence subscription request recieved -> approving subscription.";
    Presence::ref response = Presence::create();
    response->setTo(presence->getFrom());
    response->setType(Presence::Subscribed);
    client_->sendPresence(response);
  } /*else if (presence->getType() == Presence::Subscribed) {

  } else if (presence->getType() == Presence::Unsubscribe) {

  } else if (presence->getType() == Presence::Unsubscribed) {

  }*/
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

void BcModel::handleJidUpdated(const JID &jid, const string &/*name*/, const vector<string> &/*groups*/) {
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

void BcModel::UnsubscribeFromChannel(const Swift::JID &jid) {
  unsubscriber_.Discover(jid, client_->getIQRouter());
}

void BcModel::SubscribeToChannel(const Swift::JID &jid) {
  subscriber_.Discover(jid, client_->getIQRouter());
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

Channel* BcModel::CreateChannel(const Swift::JID &jid) {
  if (channels_map_.count(jid) > 0)
    return channels_map_[jid];
  Channel* channel = new Channel(this, jid);
  channels_.push_back(channel);
  channels_map_[jid] = channel;
  return channel;
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

int BcModel::GetStatus(Presence::ref presence) {
  int result;
  switch (presence->getType()) {
    case Presence::Unavailable:
      result = Contact::Offline;
      break;
    case Presence::Available:
      result = Contact::Online;
      break;
    default:
      result = Contact::Offline;
      break;
  }
  return result;
}
