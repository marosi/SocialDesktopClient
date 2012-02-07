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

#include "sdc.h"

#include "Swiften/Swiften.h"
#include "boost/bind.hpp"

#include "pubsub_payload.h"
#include "pubsub_payload_parser.h"
#include "pubsub_payload_serializer.h"

class BuddycloudConnection;

class PubsubRequest : public Swift::GenericRequest<PubsubPayload> {
 public:
  PubsubRequest(const std::string &body, const std::string &reciever, Swift::IQRouter* router) :
      Swift::GenericRequest<PubsubPayload>(Swift::IQ::Get, reciever, boost::shared_ptr<PubsubPayload>(new PubsubPayload()), router) {}
};

class BuddycloudBot {
  public:
    BuddycloudBot(BuddycloudConnection* connection, Swift::NetworkFactories* networkFactories);
    ~BuddycloudBot();

    void SendMessage(std::string msg);
    void SendDiscoInfo(const std::string &to_attribute, const std::string &node_attribute);
    void SendDiscoItems(const std::string &to_attribute, const std::string &node_attribute);

  private:
    void handleConnected();
    void handleIQRecieved(boost::shared_ptr<Swift::IQ> iq);
    void handleMessageReceived(Swift::Message::ref message);
    void handlePresenceReceived(Swift::Presence::ref presence);
    void handleRosterReceived(Swift::ErrorPayload::ref error);
    void handleDataRecieved(const Swift::SafeByteArray &byte_array);

  private:
    BuddycloudConnection* connection_;
    Swift::Client* client;
    Swift::ClientXMLTracer* tracer;

    PubsubPayloadParserFactory pubsubPayloadParserFactory_;
    PubsubPayloadSerializer pubsubPayloadSerializer_;
};


#endif /* BUDDYCLOUD_BOT_H_ */
