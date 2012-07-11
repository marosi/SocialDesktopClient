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

#include "post.h"
#include "bc_contact.h"

#include "payloads/pubsub.h"

#include "sdc.h"
#include "Swiften/Client/Client.h"
#include "Swiften/Client/ClientXMLTracer.h"
#include "Swiften/EventLoop/SimpleEventLoop.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Network/NetworkFactories.h"
#include "boost/signals2.hpp"
#include <vector>
#include <set>

#include "channel.h" // move to cc as soon as ChannelServiceInfo is resolved

class BcModel : public sdc::QtServiceModel {
  public:
    friend class BuddycloudConnection;
    friend class ChannelController;

    BcModel(sdc::AccountData* account);
    ~BcModel();
    sdc::Connection* CreateConnection() {
      connection_ = new BuddycloudConnection(this);
      return connection_;
    }

    //GetSoftwareVersionRequest::ref gsvr = GetSoftwareVersionRequest::create("localhost", client_->getIQRouter());
    //GetVCardRequest::ref gvcr = GetVCardRequest::create(to, client_->getIQRouter());
    void SendDiscoInfo(const std::string &to_attribute, const std::string &node_attribute);
    void SendDiscoItems(const std::string &to_attribute, const std::string &node_attribute);


    Swift::Client* xmpp() {
      return client_;
    }

    void ToggleChannelPrivacy();
    void Unsubscribe();

    /**
     * Content interface
     */
    BcContact::Ref GetContact(const Swift::JID &jid);
    ChannelController* GetChannel(const Swift::JID &jid);
    void AddNewContact(BcContact::Ref contact);
    void RemoveContact(BcContact::Ref contact);
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
    boost::signals2::signal<void ()> onConnected;

    boost::signals2::signal<void (const std::vector<BcContact::Ref>) > onContactsReady;
    boost::signals2::signal<void ()> onSelfChannelRegistered;

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
    std::map<Swift::JID, ChannelController*> channels_;
    /**
     * Channel client data
     */
    Swift::JID jid_;
    std::string posts_node_;
    ChannelServiceInfo channel_service_;
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
