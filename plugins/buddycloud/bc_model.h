/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BC_MODEL_H_
#define BC_MODEL_H_

#include "discover_service.h"
#include "sdc/qtgui/qt_service.h"
#include "sdc/qtgui/qt_service_model.h"
#include "Swiften/Client/Client.h"
#include "Swiften/Client/ClientXMLTracer.h"
#include "Swiften/Elements/DiscoInfo.h"
#include "Swiften/Elements/Message.h"
#include "Swiften/Elements/Presence.h"
#include "Swiften/Elements/VCard.h"
#include "Swiften/EventLoop/SimpleEventLoop.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Network/NetworkFactories.h"
#include "Swiften/Roster/XMPPRosterItem.h"
#include "boost/signals2.hpp"
#include <vector>
#include <set>
#include <string>

class BcContact;
class Channel;
class Comment;
class FilesystemStorages;
class Post;

struct Activity {
  enum Verb {
    Note,
    Comment
  };

  Swift::JID from;
  Verb verb;
  Swift::JID to;
  std::string to_;
};

class BcModel : public sdc::QtServiceModel {
  public:
    friend class BuddycloudConnection;
    friend class Channel;

    BcModel(sdc::Account* account);
    ~BcModel();

    void Run();
    void Stop();
    void Connect();
    void Disconnect();

    /**
     * Subscribe to channel owned by jid.
     * @param jid channel owner
     */
    void SubscribeToChannel(const Swift::JID &jid);
    /**
     * Unsubscribe from channel owned by jid.
     * @param jid channel owner
     */
    void UnsubscribeFromChannel(const Swift::JID &jid);

    void ToggleChannelPrivacy();

    /*
     * Content interface
     */
    BcContact* GetContact(const Swift::JID &jid);
    Swift::JID GetOwnJID() { return jid_; }
    Channel* GetChannel(const Swift::JID &jid);
    Channel* GetOwnChannel() { return own_channel_; }

    void AddNewContact(const Swift::JID &jid);

    const std::string GetOwnAvatarPath();
    const std::string GetAvatarPath(const Swift::JID &jid);
    const std::string GetDefaultAvatarPath();
    /*
     * Errors
     */
    enum Error {
      ServiceUnavailable
    };
    /*
     * Signals
     */
    boost::signals2::signal<void (Error)> onError;
    boost::signals2::signal<void ()> onConnected;
    boost::signals2::signal<void ()> onDisconnected;

    boost::signals2::signal<void ()> onSelfChannelRegistered;
    boost::signals2::signal<void ()> onChannelAvailable;

    boost::signals2::signal<void (const Swift::JID)> onContactAdded;
    boost::signals2::signal<void (const Swift::JID)> onContactRemoved;

    boost::signals2::signal<void (Activity)> onNewActivity;

    boost::signals2::signal<void (const Swift::JID)> onAvatarChanged;

  private:
    /*
     * On request
     */
    void GetServerInfo();
    /*
     * Connected/disconnected handlers
     */
    void handleConnected();
    void handleDisconnected(boost::optional<Swift::ClientError> error);
    /*
     * Permanent stanza handling
     */
    void handleMessageReceived(Swift::Message::ref message);
    void handlePresenceReceived(Swift::Presence::ref presence);
    /*
     * Roster change handlers
     */
    void handleJidAdded(const Swift::JID &jid);
    void handleJidRemove(const Swift::JID &jid);
    void handleJidUpdated(const Swift::JID &jid, const std::string &name, const std::vector<std::string> &groups);
    /*
     * Parsers and serializers
     */
    void AddParserFactories();
    void AddSerializers();
    void AddParserFactory(Swift::PayloadParserFactory* factory);
    void AddSerializer(Swift::PayloadSerializer* serializer);
    void AddContact(const Swift::XMPPRosterItem &item);
    Channel* CreateChannel(const Swift::JID &jid);
    /*
     * Miscellaneous
     */
    const std::string SavePhoto(const Swift::JID &jid, Swift::VCard::ref vcard);
    int GetStatus(Swift::Presence::ref presence);
    /**
     * SDC data
     */
    sdc::Account* account_;
    std::vector<BcContact*> contacts_;
    std::map<Swift::JID, BcContact*> contacts_map_;
    /**
     * Domain server stuff
     */
    Swift::DiscoInfo::ref server_info_;
    /**
     * Channels
     */
    Channel* own_channel_;
    std::vector<Channel*> channels_;
    std::map<Swift::JID, Channel*> channels_map_;
    /**
     * Channel client data
     */
    Swift::JID jid_;
    Swift::JID service_jid_;
    bool is_service_registration_available_;
    /**
     * Switen engine
     */
    Swift::SimpleEventLoop* loop_;
    Swift::NetworkFactories* network_;
    FilesystemStorages* storages_;
    Swift::Client* client_;
    Swift::ClientXMLTracer* tracer_;
    /**
     * Serializers & Parsers
     */
    std::vector<Swift::PayloadParserFactory*> parsers_;
    std::vector<Swift::PayloadSerializer*> serializers_;

    DiscoverService subscriber_;
    DiscoverService unsubscriber_;
};

#endif // BC_MODEL_H_
