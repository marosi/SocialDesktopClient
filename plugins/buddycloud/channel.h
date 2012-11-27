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

class Channel : public AbstractModel {
 public:
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

  struct ChannelUserInfo {
    bool is_user_channel_registered; // TODO: deprecate, only client can have this state!

    Swift::JID jid;
    std::string posts_node;
    std::string subscription_node;

    ChannelUserInfo(
        bool is_user_channel_registered = false)
        : is_user_channel_registered(is_user_channel_registered) {}
  };

  friend class BcModel;
  typedef boost::shared_ptr<Channel> ref;

  enum Error {
    ChannelsServiceUnavailable,
    UserChannelNotPresent
  };

  enum Nodes {
    Posts,
    Status,
    Subscriptions,
    GeoCurrent,
    GeoPrevious,
    GeoFuture
  };

  enum AccessModel {
    Open,
    Whitelist
  };

  enum PublishModel {
    Publishers,
    Subscribers
  };

  Channel(BcModel* bot, const Swift::JID &jid);
  ~Channel();

  void Sync();
  void RetrieveNextPosts();
  void PublishPost(const std::string &content);
  void PublishComment(const std::string &commented_post_id, const std::string &content);
  void DeletePost(Post* post); // TODO:

  const std::vector<Post*> posts() const {
    return posts_;
  }

  Swift::JID GetChannelServiceJid() const {
    return service_.jid;
  }

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

 private:
  void DiscoverChannel();
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
  std::string geo_current_node_;
  std::string geo_previous_node_;
  std::string geo_future_node_;
  std::vector<Post*> posts_;
  std::map<std::string, Post*> posts_map_;
  std::string pagination_;
  std::string first_post_id_;
  std::string last_post_id_;
  std::string posts_count_;
};

#endif // CHANNEL_H_
