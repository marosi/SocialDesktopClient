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
#include "pubsub_parser.h"
#include "pubsub_serializer.h"
#include "sdc.h"
#include "Swiften/Swiften.h"
#include "boost/bind.hpp"

class BuddycloudConnection;

class PubsubRequest : public Swift::GenericRequest<Pubsub> {
 public:
  typedef boost::shared_ptr<PubsubRequest> ref;

  static ref createPublish(const std::string &reciever, const std::string &node, Swift::Payload::ref payload, Swift::IQRouter* router) {
    Pubsub::ref pubsub(new Pubsub(Pubsub::PUBLISH, node));
    pubsub->setPublishItem(payload);
    return ref(new PubsubRequest(pubsub, reciever, router));
  }

  static ref createSubscribe(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
    Pubsub::ref pubsub(new Pubsub(Pubsub::SUBSCRIBE, node));
    return ref(new PubsubRequest(pubsub, reciever, router));
  }

  static ref createUnsubscribe(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
    Pubsub::ref pubsub(new Pubsub(Pubsub::UNSUBSCRIBE, node));
    return ref(new PubsubRequest(pubsub, reciever, router));
  }

 private:
  PubsubRequest(Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<Pubsub>(Swift::IQ::Set, reciever, payload, router) {}

  virtual void handleResponse(boost::shared_ptr<Pubsub> payload, Swift::ErrorPayload::ref error) {
    LOG(DEBUG) << "Pubsub publish RESPOND";
  }
};

struct ChannelServiceInfo {
  std::string jid;
  bool is_available;
  bool is_registration_available;

  ChannelServiceInfo(
      std::string jid = "",
      bool is_avail = false,
      bool is_registration = false)
      : jid(jid),
        is_available(is_available),
        is_registration_available(is_registration) {}
};

struct ChannelUserInfo {
  bool is_user_channel_registered;
  std::string jid;

  ChannelUserInfo(
      bool is_user_channel_registered = false,
      std::string jid = "")
      : is_user_channel_registered(is_user_channel_registered),
        jid(jid) {}
};

class BuddycloudBot {
  public:
    friend class BuddycloudConnection;

    BuddycloudBot(BuddycloudConnection* connection, Swift::NetworkFactories* networkFactories);
    ~BuddycloudBot();

    void SendMessage(std::string msg);
    void SendDiscoInfo(const std::string &to_attribute, const std::string &node_attribute);
    void SendDiscoItems(const std::string &to_attribute, const std::string &node_attribute);
    void DoSomething(const std::string &param);

    // XMPP pubsub and channel protocol abstraction
    void DiscoverChannelService() {
      // TODO: IMPLEMENT CHANNEL DISCOVERY
      channel_service_.is_available = true;
      channel_service_.is_registration_available = true;
      channel_service_.jid = "buddycloud.example.com";
    }
    void DiscoverUserSelfChannel() {
      // TODO: IMPLEMENT CHANNEL USER DISCOVERY
      channel_user_.is_user_channel_registered = true;
      channel_user_.jid = "pista@localhost";
    }
    void RegisterChannel() {} // TODO: done by inbandregistration to channel component
    void PublishAtomToNode(const std::string &node, Atom::ref atom) {
      PubsubRequest::ref publish = PubsubRequest::createPublish(channel_service_.jid, node, atom, client_->getIQRouter());
      publish->send();
    }
    void SubscribeToNode(const std::string &node) {}
    void RetrieveNodeItems() {}

    // higher abstract level
    void SubscribeToChannel(const std::string &channel) {}

    Swift::Client* client() {
      return client_;
    }

  private:
    void handleConnected();
    void handleIQRecieved(boost::shared_ptr<Swift::IQ> iq);
    void handleMessageReceived(Swift::Message::ref message);
    void handlePresenceReceived(Swift::Presence::ref presence);
    void handleRosterReceived(Swift::ErrorPayload::ref error);
    void handleDataRecieved(const Swift::SafeByteArray &byte_array);
    void handleRegisterChannel() {}

  private:
    void AddParserFactories();
    void AddSerializers();
    void AddParserFactory(Swift::PayloadParserFactory* factory);
    void AddSerializer(Swift::PayloadSerializer* serializer);

    BuddycloudConnection* connection_;
    Swift::Client* client_;
    Swift::ClientXMLTracer* tracer_;

    std::vector<Swift::PayloadParserFactory*> parsers_;
    std::vector<Swift::PayloadSerializer*> serializers_;

    ChannelServiceInfo channel_service_;
    ChannelUserInfo channel_user_;
};


#endif /* BUDDYCLOUD_BOT_H_ */
