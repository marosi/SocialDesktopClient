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

using namespace Swift;
using namespace boost;

class BuddycloudConnection;

class PubsubRequest : public Swift::GenericRequest<PubsubPayload> {
 public:
  PubsubRequest(const std::string &body, const std::string &reciever, Swift::IQRouter* router) :
      Swift::GenericRequest<PubsubPayload>(Swift::IQ::Get, reciever, boost::shared_ptr<PubsubPayload>(new PubsubPayload()), router) {}
};

class BuddycloudBot {
  public:
    BuddycloudBot(BuddycloudConnection* connection, NetworkFactories* networkFactories);
    ~BuddycloudBot();

    void SendMessage(std::string msg);

  private:
    void handleConnected();
    void handleMessageReceived(Message::ref message);
    void handlePresenceReceived(Presence::ref presence);
    void handleRosterReceived(ErrorPayload::ref error);

  private:
    BuddycloudConnection* connection_;
    Client* client;
    ClientXMLTracer* tracer;

    PubsubPayloadParserFactory pubsubPayloadParserFactory_;
    PubsubPayloadSerializer pubsubPayloadSerializer_;
};


#endif /* BUDDYCLOUD_BOT_H_ */
