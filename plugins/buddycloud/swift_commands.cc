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
  signal_connection_ = conn->bot()->client()->onConnected.connect(bind(&SwiftGoOnlineRequest::HandleResponse, this));
}

void SwiftGoOnlineRequest::HandleResponse() {
  signal_connection_.disconnect();
  HandleContent(sdc::Content::Ref());
}

void SwiftContactsRequest::HandleRequest(sdc::Connection* connection) {
  BuddycloudConnection* conn = boost::polymorphic_downcast<BuddycloudConnection*>(connection);
  assert(conn);
  GetRosterRequest::ref request = GetRosterRequest::create(conn->bot()->client()->getIQRouter());
  request->onResponse.connect(bind(&SwiftContactsRequest::HandleResponse, this, _1, _2));
  request->send();
}

void SwiftContactsRequest::HandleResponse(Payload::ref payload, ErrorPayload::ref error) {
  RosterPayload::ref roster = dynamic_pointer_cast<RosterPayload> (payload);
  assert(roster);
  vector<RosterItemPayload> roster_items = roster->getItems();
  vector<RosterItemPayload>::iterator it;
  sdc::Contacts::Ref contacts(new sdc::Contacts);
  for (it = roster_items.begin(); it != roster_items.end(); ++it) {
    sdc::Contact::Ref c(new sdc::Contact);
    c->SetUid(it->getJID().toString());
    c->SetName(it->getJID().toString());
    contacts->AddItem(c);
  }
  HandleContent(contacts);
}
