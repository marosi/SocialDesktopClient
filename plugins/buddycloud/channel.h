/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "abstract_model.h"
#include "payloads/atom.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Queries/IQRouter.h"
#include "Swiften/Elements/DiscoInfo.h"
#include "Swiften/Elements/DiscoItems.h"
#include "Swiften/Elements/ErrorPayload.h"
#include "boost/signals2/signal.hpp"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

class BcModel;
class Post;
class Comment;

/**
 * Channel model class.
 * Encapsulates business logic for Channel protocol.
 */
class Channel : public AbstractModel {
 public:
  /**
   * Structure holding channel service metadata.
   */
  struct ChannelServiceInfo {
    Swift::JID jid;
    bool is_available;
    bool is_registration_available;

    ChannelServiceInfo(
        bool is_avail = false,
        bool is_registration = false)
        : is_available(is_avail),
          is_registration_available(is_registration) {}
  };

  /**
   * Structures for channel user metadata.
   */
  struct ChannelUserInfo {
    bool is_user_channel_registered; // TODO: deprecate, only client can have this state!

    Swift::JID jid;
    std::string posts_node;
    std::string subscription_node;

    ChannelUserInfo(
        bool is_user_channel_registered = false)
        : is_user_channel_registered(is_user_channel_registered) {}
  };

  /**
   * Channel node affiliation options.
   * Corresponds with XEP-0060 affiliations, however does not complete them.
   */
  enum Affiliation {
    Owner,
    Moderator,
    Publisher,
    Member,
    Outcast,
    None  // PROTOCOL: not specified in channel protocol, however present in some subscriptions
  };

  /**
   * Channel node subscription states.
   * Corresponds with XEP-0060, however does not complete it.
   */
  enum SubscriptionState {
    Subscribed,
    Pending,
    NoneState
  };

  /**
   * Structure for the pubsub node user is subscribed to.
   */
  struct SubscribedNode {
    std::string node;
    SubscriptionState state;
    Affiliation affiliation;
  };

  /**
   * Structure for user's generic subscription.
   */
  struct Subscription {
    Swift::JID jid;
    Swift::JID channel_server;
    std::vector<SubscribedNode> nodes;
  };

  friend class BcModel;
  typedef boost::shared_ptr<Channel> ref;

  /**
   * Enumeration of channel error states.
   */
  enum Error {
    ChannelsServiceUnavailable,
    UserChannelNotPresent
  };

  /**
   * Enumeration of available channel nodes.
   */
  enum Nodes {
    Posts,
    Status,
    Subscriptions,
    GeoCurrent,
    GeoPrevious,
    GeoFuture
  };

  /*
   * Getters for channel nodes.
   */
  static std::string GetPostsNode(const Swift::JID &jid);
  static std::string GetStatusNode(const Swift::JID &jid);
  static std::string GetSubscriptionsNode(const Swift::JID &jid);
  static std::string GetGeoCurrentNode(const Swift::JID &jid);
  static std::string GetGeoNextNode(const Swift::JID &jid);
  static std::string GetGeoPreviousNode(const Swift::JID &jid);

  Channel(BcModel* bot, const Swift::JID &jid);
  ~Channel();

  /**
   * Synchronizes user channel content within the client.
   */
  void Sync();

  /**
   * Sequentialy retrieves posts.
   */
  void RetrieveNextPosts();

  /**
   * Publishes topic post.
   * @param content topic content
   */
  void PublishPost(const std::string &content);

  /**
   * Publishes comment post.
   * @param commented_post_id in reply to post with ID
   * @param content comment content
   */
  void PublishComment(const std::string &commented_post_id, const std::string &content);

  /**
   * Deletes particular post.
   * TODO: unfinished logic
   * @param post
   */
  void DeletePost(Post* post);

  /**
   * Retrieve channel subscriptions.
   */
  void RetrieveSubscriptions();

  /**
   * Adds subscription to internal structures.
   * @param subscription
   */
  void AddSubscription(Subscription subscription);

  /**
   * Gets all retrieved channel posts.
   * @return vector of posts
   */
  const std::vector<Post*> posts() const {
    return posts_;
  }

  /**
   * Gets user channel's JID.
   * @return JID
   */
  Swift::JID GetChannelServiceJid() const {
    return service_.jid;
  }

  /**
   * Returns channel registration availability state.
   * @return true, if In-Band registration is available, false otherwise
   */
  bool IsRegisterationAvailable() const {
    return service_.is_registration_available;
  }

  const Swift::JID& JID() const {
    return jid_;
  }

  boost::signals2::signal<void (Error)> onError;
  boost::signals2::signal<void (ChannelServiceInfo)> onChannelsServiceAvailable;
  boost::signals2::signal<void ()> onChannelAvailable;
  boost::signals2::signal<void (const std::vector<Post*>) > onNewPostsRetrieved;
  boost::signals2::signal<void (const std::string)> onChannelTitleChange;
  boost::signals2::signal<void (const std::string)> onChannelDescriptionChange;
  boost::signals2::signal<void (const std::string)> onPostDeleted;
  boost::signals2::signal<void (Post*)> onPostAdded;
  boost::signals2::signal<void (std::map<Swift::JID, Subscription>)> onSubscriptionsRetrieved;
  boost::signals2::signal<void (Subscription)> onNewSubscription;

 private:
  /**
   * Discovers user's channel.
   */
  void DiscoverChannel();
  /**
   * Discovers channel component within user's XMPP server.
   */
  void DiscoverService();
  /**
   * When searching for a channel service this handler is used to recursively iterate through domain items.
   */
  void handleDomainItemInfo(Swift::DiscoInfo::ref payload, Swift::ErrorPayload::ref error, boost::shared_ptr<Swift::DiscoItems> items, std::vector<Swift::DiscoItems::Item>::const_iterator it);
  /*
   * Internal structures manipulation
   */
  Post* AddPost(Atom::ref, bool signal = true);
  void RemovePost(const std::string &id);
  Post* GetPost(const std::string &id);
  /*
   * Service model vars
   */
  Swift::IQRouter* router_;
  BcModel* model_;
  /*
   * Channel service
   */
  ChannelServiceInfo service_;
  /*
   * Channel data
   */
  Swift::JID jid_;
  std::string null_node_;
  std::string posts_node_;
  std::string posts_title_;
  std::string posts_description_;
  std::string status_node_;
  std::string subscription_node_;
  std::vector<Post*> posts_;
  std::map<std::string, Post*> posts_map_;
  std::string pagination_;
  std::string first_post_id_;
  std::string last_post_id_;
  std::string posts_count_;
  std::map<Swift::JID, Subscription> subscriptions_;
};

#endif // CHANNEL_H_
