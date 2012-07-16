/*
 * channel.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 7, 2012
 */

#include "channel.h"
#include "bc_model.h"
#include "post.h"
#include "payloads/pubsub.h"
#include "pubsub_requests.h"
#include <algorithm>

using namespace Swift;
using namespace boost;
using std::string;
using std::vector;

/*
 * Channel controller
 */
ChannelController::ChannelController(BcModel* model, const Swift::JID &jid)
    : AbstractModel(model),
      router_(model->client_->getIQRouter()),
      model_(model),
      jid_(jid) {
  null_node_          = "/user/" + jid_.toString();
  posts_node_         =  null_node_ + "/posts";
  status_node_        =  null_node_ + "/status";
  subscription_node_  =  null_node_ + "/subscription";
  geo_current_node_   =  null_node_ + "/geo/current";
  geo_previous_node_  =  null_node_ + "/geo/previous";
  geo_future_node_    =  null_node_ + "/geo/future";

  // default pagination of items requests
  pagination_ = "20";
  first_post_id_ = "";
  last_post_id_ = "";
}

ChannelController::~ChannelController() {
  for (Post1* p : posts_)
    delete p;
}

void ChannelController::Sync() {
  // When service is available, check if there is any channel for JID ...
  onChannelsServiceAvailable.connect(bind(&ChannelController::DiscoverChannel, this));
  // ... if there is, retrieve first page of posts.
  onChannelAvailable.connect(bind(&ChannelController::RetrieveNextPosts, this));
  DiscoverService();
}

void ChannelController::RetrieveNextPosts() {
  GetPubsubItemsRequest::ref request = GetPubsubItemsRequest::create(last_post_id_, pagination_, posts_node_, service_.jid, router_);
  request->onResponse.connect([&] (PubsubItemsRequest::ref items, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    // obtain channel posts
    vector<Post1*> new_posts;
    for (const Atom::ref &atom : items->getItems()->get()) {
      if (atom->getInReplyTo() == "") {
        Post1* post = new Post1(this);
        post->SetID(atom->getID());
        post->SetAuthor(atom->getAuthor());
        post->SetContent(atom->getContent());
        new_posts.push_back(post);
        AddPost(post, false);
      }
    }
    // obtain channel comments
    for (const Atom::ref &atom : items->getItems()->get()) {
      if (atom->getInReplyTo() != "") {
        Post1* post = GetPost(atom->getInReplyTo());
        if (post) {
          Comment* comment = new Comment(post, atom->getContent());
          comment->SetAuthor(atom->getAuthor());
          comment->SetID(atom->getID());
          post->AddComment(comment, false);
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

void ChannelController::CreatePost(Post1* post) {
  post->SetAuthor(model_->jid_.toString());
  // translate post to atom
  Atom::ref atom(new Atom);
  atom->setAuthor(post->GetAuthor());
  atom->setContent(post->GetContent());
  atom->setObjectType(Atom::NOTE);
  atom->setVerb(Atom::POST);
  // create request
  PubsubPublishRequest::ref publish(new PubsubPublishRequest);
  publish->setPayload(atom);
  publish->setNode(posts_node_);
  SetPubsubPublishRequest::ref rq = SetPubsubPublishRequest::create(publish, service_.jid, router_);
  // on response actions
  rq->onResponse.connect([=] (PubsubPublishRequest::ref payload, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    post->SetID(payload->getItemID());
    AddPost(post);
  });
  rq->send();
}

void ChannelController::CreateComment(Comment* comment) {
  comment->SetAuthor(model_->jid_.toString());
  // translate post to atom
  Atom::ref atom(new Atom);
  atom->setAuthor(comment->GetAuthor());
  atom->setContent(comment->GetContent());
  atom->setInReplyTo(comment->GetCommentedID());
  atom->setObjectType(Atom::COMMENT);
  atom->setVerb(Atom::POST);
  // create request
  PubsubPublishRequest::ref publish(new PubsubPublishRequest);
  publish->setPayload(atom);
  publish->setNode(posts_node_);
  SetPubsubPublishRequest::ref rq = SetPubsubPublishRequest::create(publish, service_.jid, router_);
  // on response actions
  rq->onResponse.connect([=] (PubsubPublishRequest::ref payload, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    Post1* post = GetPost(comment->GetCommentedID());
    if (post) {
      comment->SetID(payload->getItemID());
      post->AddComment(comment);
    }
  });
  rq->send();
}

void ChannelController::DeletePost(Post1* post) {
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

void ChannelController::DiscoverChannel() {
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
        for (Form::ref form : payload->getExtensions()) {
          /*string field("pubsub#title");
          LOG(DEBUG) << "Node info form";
          LOG(DEBUG) << "Description: " << form->getField(field)->getDescription();
          LOG(DEBUG) << "Label: " << form->getField(field)->getLabel();
          LOG(DEBUG) << "Name: " << form->getField(field)->getName();
          LOG(DEBUG) << "RawValues size: " << form->getField(field)->getRawValues().size();
          for (const string &s : form->getField(field)->getRawValues()) {
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

void ChannelController::DiscoverService() {
  GetDiscoItemsRequest::ref items = GetDiscoItemsRequest::create(jid_.getDomain(), router_);
  items->onResponse.connect([&] (shared_ptr<DiscoItems> payload, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    LOG(TRACE) << "Channel " << jid_ << " : handling domain disco items ... ";
    vector<DiscoItems::Item>::const_iterator it = payload->getItems().begin();
    GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), router_);
    info->onResponse.connect(bind(&ChannelController::handleDomainItemInfo, this, _1, _2, payload, it));
    info->send();
  });
  items->send();
}

void ChannelController::handleDomainItemInfo(DiscoInfo::ref payload, ErrorPayload::ref error, shared_ptr<DiscoItems> items, vector<DiscoItems::Item>::const_iterator it) {
  if (error) {
    LOG(ERROR) << error->getText();
  } else {
    for (const DiscoInfo::Identity &identity : payload->getIdentities()) {
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
      info->onResponse.connect(bind(&ChannelController::handleDomainItemInfo, this, _1, _2, items, it));
      info->send();
    }
  }
}

/*
 * Internal structures manipulation
 */

void ChannelController::AddPost(Post1* post, bool signal) {
  if (id_posts_.count(post->GetID()) == 0) {
    posts_.push_back(post);
    id_posts_[post->GetID()] = post;
    if (signal)
      onPostAdded(post);
  }
}

void ChannelController::RemovePost(const std::string &id) {
  if (id_posts_.count(id) > 0) {
    vector<Post1*>::iterator it = std::find_if(posts_.begin(), posts_.end(), [&] (const Post1* p) { return p->GetID() == id; });
    if (it != posts_.end())
      posts_.erase(it);
    id_posts_.erase(id);
    onPostDeleted(id);
  }
}

Post1* ChannelController::GetPost(const std::string &id) {
  if (id_posts_.count(id) > 0)
    return id_posts_[id];
  else
    return 0;
}
