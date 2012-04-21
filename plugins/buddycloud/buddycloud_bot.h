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

#include "pubsub.h"
#include "sdc.h"
#include "Swiften/Swiften.h"
#include "boost/bind.hpp"
#include "boost/signals2.hpp"

class BuddycloudConnection;

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

class BuddycloudBot {
  public:
    friend class BuddycloudConnection;

    BuddycloudBot(const std::string &jid, const std::string &password);
    ~BuddycloudBot();

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
     * Errors
     */
    enum Error {
      ServiceUnavailable
    };

    boost::signals2::signal1<void, Error> onError;
    boost::signals2::signal1<void, Swift::RosterPayload::ref> onRosterReady;

  private:
    void handleConnected();
    void handleIQRecieved(boost::shared_ptr<Swift::IQ> iq);
    void handleMessageReceived(Swift::Message::ref message);
    void handlePresenceReceived(Swift::Presence::ref presence);
    void handleRosterReceived(Swift::RosterPayload::ref payload, Swift::ErrorPayload::ref error);
    void handleDataRecieved(const Swift::SafeByteArray &byte_array);
    void handleRegisterChannel() {}

    void handleItemsRecieved(boost::shared_ptr<PubsubItemsRequest> items); // TODO: DELETE after test


    void AddParserFactories();
    void AddSerializers();
    void AddParserFactory(Swift::PayloadParserFactory* factory);
    void AddSerializer(Swift::PayloadSerializer* serializer);

    /**
     * Channel handling
     */
    void handleServerDiscoItems(boost::shared_ptr<Swift::DiscoItems> payload, Swift::ErrorPayload::ref error);
    void handleServerDiscoItemInfo(Swift::DiscoInfo::ref payload, Swift::ErrorPayload::ref error, boost::shared_ptr<Swift::DiscoItems> items, std::vector<Swift::DiscoItems::Item>::const_iterator it);
    void UserChannelDiscovery();
    void handleUserChannelDiscovery(Swift::DiscoInfo::ref payload, Swift::ErrorPayload::ref error);
    void handleChannelRegistration(Swift::Payload::ref payload, Swift::ErrorPayload::ref error);
    /**
     * Signals
     */

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
    /**
     * Serializers & Parsers
     */
    std::vector<Swift::PayloadParserFactory*> parsers_;
    std::vector<Swift::PayloadSerializer*> serializers_;
};


#endif /* BUDDYCLOUD_BOT_H_ */
