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

using namespace Swift;
using namespace boost;

class BuddycloudBot {
  public:
    BuddycloudBot(NetworkFactories* networkFactories) {
      client = new Client("sdc_test@jabbim.com", "sdc_test", networkFactories);
      client->setAlwaysTrustCertificates();
      client->onConnected.connect(bind(&BuddycloudBot::handleConnected, this));
      client->onMessageReceived.connect(
          bind(&BuddycloudBot::handleMessageReceived, this, _1));
      client->onPresenceReceived.connect(
                bind(&BuddycloudBot::handlePresenceReceived, this, _1));
      tracer = new ClientXMLTracer(client);
      client->connect();
    }

    ~BuddycloudBot  () {
      delete tracer;
      delete client;
    }

  private:
    void handleConnected() {
      std::cout << "@@@ CLIENT IS CONNECTED @@@" << std::endl;
      // Request the roster
      GetRosterRequest::ref rosterRequest =
          GetRosterRequest::create(client->getIQRouter());
      rosterRequest->onResponse.connect(
          bind(&BuddycloudBot::handleRosterReceived, this, _2));
      rosterRequest->send();
    }

    void handleMessageReceived(Message::ref message) {

      LOG(DEBUG) << "Handling recieved message..";

      // Echo back the incoming message
      if (message->getBody() != "") {
        std::cout << message->getBody() << std::endl;
        message->setTo(message->getFrom());
        message->setFrom(JID());
        client->sendMessage(message);

        // Transform message into SDC core message format

        // Post message onto EventLoop queue
        sdc::SimpleStringMessage* msg = new sdc::SimpleStringMessage(message->getBody());
        sdc::g_event_manager->PostMessage(msg);
      }
    }

    void handlePresenceReceived(Presence::ref presence) {

      LOG(DEBUG) << "Handling presence message..";

          // Automatically approve subscription requests
          if (presence->getType() == Presence::Subscribe) {
            Presence::ref response = Presence::create();
            response->setTo(presence->getFrom());
            response->setType(Presence::Subscribed);
            client->sendPresence(response);
          }
        }

    void handleRosterReceived(ErrorPayload::ref error) {
          if (error) {
            std::cerr << "Error receiving roster. Continuing anyway.";
          }
          // Send initial available presence
          client->sendPresence(Presence::create("Send me a message"));
        }

  private:
    Client* client;
    ClientXMLTracer* tracer;
};


#endif /* BUDDYCLOUD_BOT_H_ */
