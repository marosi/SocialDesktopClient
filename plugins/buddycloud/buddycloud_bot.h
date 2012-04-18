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
    return ref(new PubsubRequest(Swift::IQ::Set, pubsub, reciever, router));
  }

  static ref createSubscribe(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
    Pubsub::ref pubsub(new Pubsub(Pubsub::SUBSCRIBE, node));
    return ref(new PubsubRequest(Swift::IQ::Set, pubsub, reciever, router));
  }

  static ref createUnsubscribe(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
    Pubsub::ref pubsub(new Pubsub(Pubsub::UNSUBSCRIBE, node));
    return ref(new PubsubRequest(Swift::IQ::Set, pubsub, reciever, router));
  }

  static ref createItems(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
    Pubsub::ref pubsub(new Pubsub(Pubsub::ITEMS, node));
    return ref(new PubsubRequest(Swift::IQ::Get, pubsub, reciever, router));
  }

 private:
  PubsubRequest(Swift::IQ::Type type, Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<Pubsub>(type, reciever, payload, router) {}
};

class GetPubsubItemsRequest : public Swift::GenericRequest<PubsubItemsRequest> {
 public:
  typedef boost::shared_ptr<GetPubsubItemsRequest> ref;

  static ref create(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
    boost::shared_ptr<PubsubItemsRequest> payload(new PubsubItemsRequest);
    payload->setNode(node);
    return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
  }

  virtual void handleResponse(boost::shared_ptr<Swift::Payload> payload, Swift::ErrorPayload::ref error) {
    LOG(DEBUG) << "GetPubsubItemsRequest response object: " << typeid(*payload.get()).name();
    Swift::GenericRequest<PubsubItemsRequest>::handleResponse(payload, error);
  }

 private:
  GetPubsubItemsRequest(Swift::IQ::Type type, Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<PubsubItemsRequest>(type, reciever, payload, router) {}
};

class SetPubsubRetractRequest : public Swift::GenericRequest<PubsubRetractRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubRetractRequest> ref;

  static ref create(const std::string &id, const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
    PubsubRetractRequest::ref payload(new PubsubRetractRequest);
    payload->setNode(node);
    payload->setItemID(id);
    return ref(new SetPubsubRetractRequest(Swift::IQ::Set, payload, reciever, router));
  }

 private:
  SetPubsubRetractRequest(Swift::IQ::Type type, Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
        : Swift::GenericRequest<PubsubRetractRequest>(type, reciever, payload, router) {}
};

class SetPubsubPublishRequest : public Swift::GenericRequest<PubsubPublishRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubPublishRequest> ref;

  static ref create(Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router) {
    return ref(new SetPubsubPublishRequest(Swift::IQ::Set, payload, reciever, router));
  }

 private:
  SetPubsubPublishRequest(Swift::IQ::Type type, Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
        : Swift::GenericRequest<PubsubPublishRequest>(type, reciever, payload, router) {}
};

struct ChannelServiceInfo {
  std::string jid;
  bool is_available;
  bool is_registration_available;

  ChannelServiceInfo(
      bool is_avail = false,
      bool is_registration = false)
      : is_available(is_available),
        is_registration_available(is_registration) {}
};

struct ChannelUserInfo {
  bool is_user_channel_registered;
  std::string jid;
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
      channel_user_.posts_node = "/user/" + channel_user_.jid + "/posts";
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

    Swift::Client* xmpp() {
      return client_;
    }

    const ChannelUserInfo& GetChannelUser() const {
      return channel_user_;
    }

    const ChannelServiceInfo& GetChannelService() const {
      return channel_service_;
    }

  private:
    void handleConnected();
    void handleIQRecieved(boost::shared_ptr<Swift::IQ> iq);
    void handleMessageReceived(Swift::Message::ref message);
    void handlePresenceReceived(Swift::Presence::ref presence);
    void handleRosterReceived(Swift::ErrorPayload::ref error);
    void handleDataRecieved(const Swift::SafeByteArray &byte_array);
    void handleRegisterChannel() {}

    void handleItemsRecieved(boost::shared_ptr<PubsubItemsRequest> items); // TODO: DELETE after test

  private:
    void AddParserFactories();
    void AddSerializers();
    void AddParserFactory(Swift::PayloadParserFactory* factory);
    void AddSerializer(Swift::PayloadSerializer* serializer);

    /**
     * Switen engine
     */
    Swift::SimpleEventLoop* loop_;
    Swift::NetworkFactories* network_;
    Swift::Client* client_;
    Swift::ClientXMLTracer* tracer_;

    std::vector<Swift::PayloadParserFactory*> parsers_;
    std::vector<Swift::PayloadSerializer*> serializers_;

    ChannelServiceInfo channel_service_;
    ChannelUserInfo channel_user_;
};


#endif /* BUDDYCLOUD_BOT_H_ */
