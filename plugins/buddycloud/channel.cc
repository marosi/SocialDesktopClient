/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "channel.h"
#include "bc_model.h"
#include "error.h"
#include "post.h"
#include "payloads/pubsub.h"
#include "pubsub_requests.h"
#include "sdc/core/log.h"
#include "Swiften/Swiften.h"
#include "boost/foreach.hpp"
#include <algorithm>

using namespace Swift;
using namespace boost;
using std::string;
using std::vector;

std::string Channel::GetPostsNode(const Swift::JID &jid) {
  return "/user/" + jid.toBare().toString() + "/posts";
}

std::string Channel::GetStatusNode(const Swift::JID &jid) {
  return "/user/" + jid.toBare().toString() + "/status";
}

std::string Channel::GetSubscriptionsNode(const Swift::JID &jid) {
  return "/user/" + jid.toBare().toString() + "/subscriptions";
}

std::string Channel::GetGeoCurrentNode(const Swift::JID &jid) {
  return "/user/" + jid.toBare().toString() + "/geo/current";
}

std::string Channel::GetGeoNextNode(const Swift::JID &jid) {
  return "/user/" + jid.toBare().toString() + "/geo/next";
}

std::string Channel::GetGeoPreviousNode(const Swift::JID &jid) {
  return "/user/" + jid.toBare().toString() + "/geo/previous";
}

/*
 * Channel controller
 */
Channel::Channel(BcModel* model, const Swift::JID &jid)
    : AbstractModel(model),
      router_(model->client_->getIQRouter()),
      model_(model),
      jid_(jid) {
  posts_node_         =  GetPostsNode(jid_);
  status_node_        =  GetStatusNode(jid_);
  subscription_node_  =  GetSubscriptionsNode(jid_);

  // default pagination of items requests
  pagination_ = "20";
  first_post_id_ = "";
  last_post_id_ = "";
}

Channel::~Channel() {
  BOOST_FOREACH (Post* p , posts_)
    delete p;
}

void Channel::Sync() {
  // When service is available, check if there is any channel for JID ...
  onChannelsServiceAvailable.connect(bind(&Channel::DiscoverChannel, this));
  // ... if there is, retrieve first page of posts.
  onChannelAvailable.connect(bind(&Channel::RetrieveNextPosts, this));
  // Send presence to channel server in case it isn't in roster.
  onChannelAvailable.connect([&] () {
    Presence::ref presence(new Presence);
    presence->setTo(service_.jid);
    model_->client_->sendPresence(presence);
  });
  DiscoverService();
}

void Channel::RetrieveNextPosts() {
  GetPubsubItemsRequest::ref request = GetPubsubItemsRequest::create(last_post_id_, pagination_, posts_node_, service_.jid, router_);
  request->onResponse.connect([&] (PubsubItemsRequest::ref items, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    // obtain channel posts
    vector<Post*> new_posts;
    BOOST_FOREACH (const Atom::ref &atom , items->getItems()->getInternal<Atom>()) {
      if (atom->getInReplyTo() == "") {
        Post* post = AddPost(atom, false);
        new_posts.push_back(post);
      }
    }
    // obtain channel comments
    BOOST_FOREACH (const Atom::ref &atom , items->getItems()->getInternal<Atom>()) {
      if (atom->getInReplyTo() != "") {
        Post* post = GetPost(atom->getInReplyTo());
        if (post) {
          post->AddComment(atom, false);
        }
      }
    }
    // result set management
    if (items->getRsm()) {
      Rsm::ref rsm = items->getRsm();
      if (!rsm->getFirst().empty())
        first_post_id_ = rsm->getFirst();
      if (!rsm->getLast().empty())
        last_post_id_ = items->getRsm()->getLast();
      if (!rsm->getCount().empty())
        posts_count_ = items->getRsm()->getCount();
    }
    // signal new posts
    onNewPostsRetrieved(new_posts);
  });
  request->send();
}

void Channel::PublishPost(const std::string &content) {
  // translate post to atom
  Atom::ref atom(new Atom);
  atom->setAuthor(model_->jid_.toString());
  atom->setContent(content);
  atom->setObjectType(Atom::Note);
  atom->setVerb(Atom::Post);
  // create request
  PubsubPublishRequest::ref publish(new PubsubPublishRequest);
  publish->setPayload(atom);
  publish->setNode(posts_node_);
  SetPubsubPublishRequest::ref rq = SetPubsubPublishRequest::create(publish, service_.jid, router_);
  // on response actions
  rq->onResponse.connect([=] (PubsubPublishRequest::ref /*payload*/, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
  });
  rq->send();
}

void Channel::PublishComment(const std::string &commented_post_id, const std::string &content) {
  Atom::ref atom(new Atom);
  atom->setInReplyTo(commented_post_id);
  atom->setAuthor(model_->jid_.toString());
  atom->setContent(content);
  atom->setObjectType(Atom::Comment);
  atom->setVerb(Atom::Post);
  // create request
  PubsubPublishRequest::ref publish(new PubsubPublishRequest);
  publish->setPayload(atom);
  publish->setNode(posts_node_);
  SetPubsubPublishRequest::ref rq = SetPubsubPublishRequest::create(publish, service_.jid, router_);
  // on response actions
  rq->onResponse.connect([=] (PubsubPublishRequest::ref /*payload*/, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
  });
  rq->send();
}

void Channel::DeletePost(Post* post) {
  SetPubsubRetractRequest::ref retract = SetPubsubRetractRequest::create(post->GetID(), posts_node_, service_.jid, router_);
  retract->onResponse.connect([=] (PubsubRetractRequest::ref /*payload*/, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    RemovePost(post->GetID());
  });
  retract->send();
}

void Channel::RetrieveSubscriptions() {
  GetDiscoItemsRequest::ref req = GetDiscoItemsRequest::create(service_.jid, subscription_node_, router_);
  req->onResponse.connect([&] (boost::shared_ptr<DiscoItems> disco, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    subscriptions_.clear();
    BOOST_FOREACH (DiscoItems::Item item, disco->getItems()) {
      Subscription subit;
      subit.jid = item.getName();
      subit.channel_server = item.getJID();
      subscriptions_[subit.jid] = subit;
    }
    onSubscriptionsRetrieved(subscriptions_);
  });
  req->send();
}

void Channel::AddSubscription(Subscription sub) {
  if (subscriptions_.count(sub.jid) <= 0) {
    subscriptions_[sub.jid] = sub;
    onNewSubscription(sub);
  }
}

void Channel::DiscoverChannel() {
  // check if there's any posts node for a user
  GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(service_.jid, posts_node_, router_);
  info->onResponse.connect(
      [&] (DiscoInfo::ref payload, ErrorPayload::ref error) {
        if (error) {
          LOG(ERROR) << error->getText();
          if (error->getCondition() == ErrorPayload::ItemNotFound) {
            onError(UserChannelNotPresent);
          }
          return;
        }
        BOOST_FOREACH (Form::ref form , payload->getExtensions()) {
          /*string field("pubsub#title");
          LOG(DEBUG) << "Node info form";
          LOG(DEBUG) << "Description: " << form->getField(field)->getDescription();
          LOG(DEBUG) << "Label: " << form->getField(field)->getLabel();
          LOG(DEBUG) << "Name: " << form->getField(field)->getName();
          LOG(DEBUG) << "RawValues size: " << form->getField(field)->getRawValues().size();
          BOOST_FOREACH (const string &s , form->getField(field)->getRawValues()) {
           LOG(DEBUG2) << "RawValue: " << s;
          }*/
        }
        // get channel title
        posts_title_ = payload->getExtensions().front()->getField("pubsub#title")->getRawValues().front();
        onChannelTitleChange(posts_title_);
        // get channel description
        posts_description_ = payload->getExtensions().front()->getField("pubsub#description")->getRawValues().front();
        onChannelDescriptionChange(posts_description_);
        // signal channel available
        onChannelAvailable();
        // subscribe to posts node TODO: is it necessary?!
        /*PubsubSubscribeRequest::ref subscribe(new PubsubSubscribeRequest);
        subscribe->setNode(posts_node_);
        subscribe->setSubscribersJID(bot_->jid_); // purely decorative, just following standard XEP-0060
        SetPubsubSubscribeRequest::ref rq = SetPubsubSubscribeRequest::create(subscribe, service_.jid, router_);
        rq->send();*/
      });
  info->send();
}

/*
 * SERVICE DISCOVERY
 */

void Channel::DiscoverService() {
  GetDiscoItemsRequest::ref items = GetDiscoItemsRequest::create(jid_.getDomain(), router_);
  items->onResponse.connect([&] (shared_ptr<DiscoItems> payload, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    if (payload->getItems().size() <= 0) {
      onError(ChannelsServiceUnavailable);
    } else {
      LOG(TRACE) << "Channel " << jid_ << " : handling domain disco items ... ";
      vector<DiscoItems::Item>::const_iterator it = payload->getItems().begin();
      GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), router_);
      info->onResponse.connect(bind(&Channel::handleDomainItemInfo, this, _1, _2, payload, it));
      info->send();
    }
  });
  items->send();
}

void Channel::handleDomainItemInfo(DiscoInfo::ref payload, ErrorPayload::ref error, shared_ptr<DiscoItems> items, vector<DiscoItems::Item>::const_iterator it) {
  if (error) {
    LOG(ERROR) << error->getText();
  } else {
    BOOST_FOREACH (const DiscoInfo::Identity &identity , payload->getIdentities()) {
      // Is channel service?
      if (identity.getCategory() == "pubsub" && identity.getType() == "channels") {
        service_.is_available = true;
        service_.jid = it->getJID();
        if (payload->hasFeature("jabber:iq:register")) {
          service_.is_registration_available = true;
        } else {
          service_.is_registration_available = false;
        }
        // Signal channel available
        onChannelsServiceAvailable(service_);
      }
    }
  }
  if (!service_.is_available) { // Go for next item if service is not found yet
    ++it;
    if (it == items->getItems().end()) {
      LOG(TRACE) << "Channel service of domain " << jid_.getDomain() << " is not available!";
      onError(ChannelsServiceUnavailable);
    } else {
      GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), router_);
      info->onResponse.connect(bind(&Channel::handleDomainItemInfo, this, _1, _2, items, it));
      info->send();
    }
  }
}

/*
 * Internal structures manipulation
 */

Post* Channel::AddPost(Atom::ref atom, bool signal) {
  if (posts_map_.count(atom->getID()) > 0) {
    return posts_map_[atom->getID()];
  } else {
    // create new post
    Post* post = new Post(this);
    post->SetID(atom->getID());
    post->SetAuthor(atom->getAuthor());
    post->SetAuthorJID(atom->getAuthorJID());
    post->SetContent(atom->getContent());
    std::string p = atom->getPublished();
    post->SetPublished(stringToDateTime(p));
    posts_.push_back(post);
    posts_map_[post->GetID()] = post;
    if (signal)
      onPostAdded(post);
    return post;
  }
}

void Channel::RemovePost(const std::string &id) {
  if (posts_map_.count(id) > 0) {
    vector<Post*>::iterator it = std::find_if(posts_.begin(), posts_.end(), [&] (const Post* p) { return p->GetID() == id; });
    if (it != posts_.end())
      posts_.erase(it);
    posts_map_.erase(id);
    onPostDeleted(id);
  }
}

Post* Channel::GetPost(const std::string &id) {
  if (posts_map_.count(id) > 0)
    return posts_map_[id];
  else
    return 0;
}
