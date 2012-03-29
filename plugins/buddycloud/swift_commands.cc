/*
 * swift_commands.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 13, 2012
 */

#include "swift_commands.h"
#include "buddycloud_connection.h"
#include "buddycloud_bot.h"
#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include <vector>

using namespace Swift;
using namespace boost;
using std::vector;

void SwiftGoOnlineRequest::HandleRequest(sdc::Connection* connection) {
  connection->Connect();
  BuddycloudConnection* conn = polymorphic_downcast<BuddycloudConnection*> (connection);
  signal_connection_ = conn->bot()->xmpp()->onConnected.connect(bind(&SwiftGoOnlineRequest::HandleResponse, this));
}

void SwiftGoOnlineRequest::HandleResponse() {
  signal_connection_.disconnect();
  HandleContent(sdc::Content::Ref());
}

void SwiftContactsRequest::HandleRequest(sdc::Connection* connection) {
  BuddycloudConnection* conn = polymorphic_downcast<BuddycloudConnection*>(connection);
  assert(conn);
  GetRosterRequest::ref request = GetRosterRequest::create(conn->bot()->xmpp()->getIQRouter());
  request->onResponse.connect(bind(&SwiftContactsRequest::HandleResponse, this, _1, _2));
  request->send();
}

void SwiftContactsRequest::HandleResponse(Payload::ref payload, ErrorPayload::ref error) {
  RosterPayload::ref roster = dynamic_pointer_cast<RosterPayload> (payload);
  assert(roster);
  vector<RosterItemPayload> roster_items = roster->getItems();
  vector<RosterItemPayload>::iterator it;
  sdc::Items<sdc::Contact>::Ref contacts(new sdc::Items<sdc::Contact>);
  for (it = roster_items.begin(); it != roster_items.end(); ++it) {
    sdc::Contact::Ref c(new sdc::Contact);
    c->SetUid(it->getJID().toString());
    c->SetName(it->getJID().toString());
    contacts->AddItem(c);
  }
  HandleContent(contacts);
}

void SwiftPostsRequest::HandleRequest(sdc::Connection* connection) {
  BuddycloudConnection* conn = polymorphic_downcast<BuddycloudConnection*>(connection);
  assert(conn);
  GetPubsubItemsRequest::ref request = GetPubsubItemsRequest::create(conn->bot()->GetChannelService().jid,
      conn->bot()->GetChannelUser().posts_node, conn->bot()->xmpp()->getIQRouter());
  request->onResponse.connect(bind(&SwiftPostsRequest::HandleResponse, this, _1, _2));
  request->send();
}

void SwiftPostsRequest::HandleResponse(Payload::ref payload, ErrorPayload::ref error) {
  PubsubItemsRequest::ref items = dynamic_pointer_cast<PubsubItemsRequest>(payload);
  assert(items);
  vector<shared_ptr<Atom> > atoms = items->getItems()->get();
  vector<shared_ptr<Atom> >::iterator it;
  sdc::Items<sdc::Post>::Ref posts(new sdc::Items<sdc::Post>);
  for (it = atoms.begin(); it != atoms.end(); ++it) {
    sdc::Post::Ref post(new sdc::Post);
    post->SetID((*it)->getID());
    post->SetAuthor((*it)->getAuthor());
    post->SetContent((*it)->getContent());
    posts->AddItem(post);
  }
  HandleContent(posts);
}

void SwiftDeletePostRequest::HandleRequest(sdc::Connection* connection) {
  BuddycloudConnection* conn = polymorphic_downcast<BuddycloudConnection*>(connection);
  assert(conn);
  SetPubsubRetractRequest::ref request = SetPubsubRetractRequest::create(
      id_,
      conn->bot()->GetChannelUser().posts_node,
      conn->bot()->GetChannelService().jid,
      conn->bot()->xmpp()->getIQRouter());
  request->onResponse.connect(bind(&SwiftDeletePostRequest::HandleResponse, this, _1, _2));
  request->send();
  LOG(DEBUG) << "Delete post request send";
}

void SwiftDeletePostRequest::HandleResponse(Swift::Payload::ref payload, Swift::ErrorPayload::ref error) {
  LOG(DEBUG) << "Delete post request handled";
}
