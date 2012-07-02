/**
 * @file buddycloud_bot.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Nov 28, 2011
 */

#ifndef BUDDYCLOUD_BOT_H_
#define BUDDYCLOUD_BOT_H_

#include "buddycloud_connection.h"
#include "bc_contact.h"
#include "channel.h"
#include "pubsub.h"

#include "sdc.h"
#include "Swiften/Swiften.h"
#include "boost/bind.hpp"
#include "boost/signals2.hpp"
#include <vector>
#include <set>

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
  bool is_user_channel_registered;
  Swift::JID jid;
  std::string posts_node;

  ChannelUserInfo(
      bool is_user_channel_registered = false)
      : is_user_channel_registered(is_user_channel_registered) {}
};

class ChannelController {
 public:
  friend class BuddycloudBot;
  typedef boost::shared_ptr<ChannelController> ref; // TODO: shared ptr or not

  ChannelController(sdc::Contact::Ref contact, BuddycloudBot* bot);
  void Start();
  void GetChannelContent();
  void Send(Post1::Ref post);
  void Delete(Post1::Ref post);

  // TODO: for testing purposes... delete afterwards
  void print() {
    LOG(DEBUG) << "priniting from controller";
  }

  enum Error {
    ChannelsServiceUnavailable,
    UserChannelNotPresent
  };

  boost::signals2::signal<void (ChannelController*)> onReady;
  boost::signals2::signal<void (Error)> onError;
  boost::signals2::signal<void (ChannelServiceInfo)> onChannelsServiceAvailable;
  boost::signals2::signal<void ()> onChannelAvailable;
 private:
  void GetNodeItems(const std::string &node);
  /*
   * Handlers
   */
  void handleDomainItems(boost::shared_ptr<Swift::DiscoItems> payload, Swift::ErrorPayload::ref error);
  void handleDomainItemInfo(Swift::DiscoInfo::ref payload, Swift::ErrorPayload::ref error, boost::shared_ptr<Swift::DiscoItems> items, std::vector<Swift::DiscoItems::Item>::const_iterator it);

  Channel::Ref channel_;
  std::string posts_node_;
  Swift::JID contact_jid_;
  sdc::Contact::Ref contact_;
  ChannelServiceInfo service_;
  Swift::IQRouter* router_;
  BuddycloudBot* bot_;
};

class BuddycloudBot : public sdc::QtServiceModel {
  public:
    friend class BuddycloudConnection;
    friend class ChannelController;

    BuddycloudBot(sdc::AccountData* account);
    ~BuddycloudBot();
    sdc::Connection* CreateConnection() {
      connection_ = new BuddycloudConnection(this);
      return connection_;
    }
    virtual void Connect() {
      if (!connected_)
        connection_->Connect();
    }

    /// @{ TODO: testing REMOVE
    void SendDiscoInfo(const std::string &to_attribute, const std::string &node_attribute);
    void SendDiscoItems(const std::string &to_attribute, const std::string &node_attribute);
    void DoSomething(const std::string &param);
    /// @}
    Swift::Client* xmpp() {
      return client_;
    }
    const ChannelUserInfo& GetChannelUser() const {
      return channel_user_;
    }
    const ChannelServiceInfo& GetChannelService() const {
      return channel_service_;
    }
    /**
     * Content interface
     */
    BcContact::Ref GetContact(const Swift::JID &jid);
    void GetContactChannel(sdc::Contact::Ref contact);
    void AddNewContact(BcContact::Ref contact);
    void RemoveContact(BcContact::Ref contact);
    /**
     * Translations
     */
    Post1::Ref TranslateAtom(const Atom::ref atom);
    /**
     * Errors
     */
    enum Error {
      ServiceUnavailable
    };
    /*
     * Signals
     */
    boost::signals2::signal<void (Error)> onError;
    boost::signals2::signal<void (Swift::RosterPayload::ref)> onRosterReady;

  private:
    /*
     * On request
     */
    void GetRoster();
    void GetServerInfo();
    /*
     * Parsers and serializers
     */
    void AddParserFactories();
    void AddSerializers();
    void AddParserFactory(Swift::PayloadParserFactory* factory);
    void AddSerializer(Swift::PayloadSerializer* serializer);
    /*
     * Handlers
     */
    void handleConnected();
    void handleDisconnected(boost::optional<Swift::ClientError> error);
    /*
     * Permanent stanza handling
     */
    void handleIQRecieved(Swift::IQ::ref iq);
    void handleMessageReceived(Swift::Message::ref message);
    void handlePresenceReceived(Swift::Presence::ref presence);
    void handleDataRecieved(const Swift::SafeByteArray &byte_array);
    /*
     * Miscellaneous
     */
    void handleJidAdded(const Swift::JID &jid);
    void handleJidRemove(const Swift::JID &jid);
    void handleJidUpdated(const Swift::JID &jid, const std::string &name, const std::vector<std::string> &groups);
    /**
     * SDC data
     */
    sdc::AccountData* account_;
    sdc::Contact::Ref me_;
    BuddycloudConnection* connection_; // TODO: Rename to ChannelConnection
    std::vector<BcContact::Ref> contacts_;
    /**
     * Domain server stuff
     */
    Swift::DiscoInfo::ref server_info_;
    /**
     * Channels
     */
    ChannelController* my_channel_;
    std::set<ChannelController*> channel_controllers_;
    /**
     * Channel client data
     */
    Swift::JID jid_;
    std::string posts_node_;
    ChannelServiceInfo channel_service_;
    ChannelUserInfo channel_user_;
    /**
     * Switen engine
     */
    Swift::SimpleEventLoop* loop_;
    Swift::NetworkFactories* network_;
    Swift::Client* client_;
    Swift::ClientXMLTracer* tracer_;
    bool connected_;
    /**
     * Serializers & Parsers
     */
    std::vector<Swift::PayloadParserFactory*> parsers_;
    std::vector<Swift::PayloadSerializer*> serializers_;
};

#endif /* BUDDYCLOUD_BOT_H_ */
