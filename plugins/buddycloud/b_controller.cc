/*
 * b_controller.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 6, 2012
 */

#include "b_controller.h"
#include "b_model.h"
#include "buddycloud_connection.h"
#include "buddycloud_view.h"
#include "contact_frame_view.h"
#include "swift_commands.h"
#include "boost/bind.hpp"

using namespace boost;
using std::string;

BController::BController() {
  channel_view_ = new BuddycloudView(this);
  ConnectView();
  channel_view_->show();
}

void BController::Initiate() {
  model_ = GetModel<BModel>();
}

void BController::ConnectView() {
  connect(this, SIGNAL(signalOnlineState(const QString &)),
      channel_view_, SLOT(ShowState(const QString &)));
  connect(this, SIGNAL(signalShowContact(const QString &, const QString &)),
      channel_view_, SLOT(ShowContact(const QString &, const QString &)));
}

void BController::SwitchOnlineState(const QString &state) {
  QString lower = state.toLower();
  if (lower == "online" && GetModel<BModel>()->GetOnlineState() == "offline") {
    this->GoOnline();
  } else if (lower == "offline" && GetModel<BModel>()->GetOnlineState() != "offline") {
    this->GoOffline();
  }
  state_to_be_acknowledged_ = lower.toStdString();
}

/// XMPP testing @{
void BController::SendDiscoInfo(const string &to_attribute, const string &node_attribute) {
  LOG(DEBUG) << "SENDING DISCO INFO";
  GetConnection<BuddycloudConnection>()->HandleSendDiscoInfo(to_attribute, node_attribute);
}

void BController::SendDiscoItems(const string &to_attribute, const string &node_attribute) {
  LOG(DEBUG) << "SENDING DISCO ITEMS";
  GetConnection<BuddycloudConnection>()->HandleSendDiscoItems(to_attribute, node_attribute);
}

void BController::DoSomeThing(const string &param) {
  GetConnection<BuddycloudConnection>()->HandleSomething(param);
}
/// @}

void BController::GoOnline() {
  SwiftGoOnlineRequest::Ref request(new SwiftGoOnlineRequest);
  request->SetCallback(boost::bind(&BController::HandleIsOnline, this));
  GetConnection()->Send(request);
  channel_view_->ShowConnecting();
}

void BController::HandleIsOnline() {
  this->GetRemoteContacts();
  //
  model_->SetOnlineState(state_to_be_acknowledged_);
  QString str = QString::fromStdString(model_->GetOnlineState());
  emit signalOnlineState(str);
}

void BController::GoOffline() {
  GetConnection<BuddycloudConnection>()->Disconnect();
  model_->SetOnlineState("offline");
}

void BController::GetRemoteContacts() {
  SwiftContactsRequest::Ref request(new SwiftContactsRequest);
  request->SetCallback(boost::bind(&BController::HandleRemoteContacts, this, _1));
  GetConnection()->Send(request);
}

void BController::HandleRemoteContacts(sdc::Contacts::Ref contacts) {
  contacts->Iterate();
  while(sdc::Contact::Ref contact = contacts->GetNext()) {
    emit signalShowContact(
        QString::fromStdString(contact->GetUid()),
        QString::fromStdString(contact->GetName()));
  }
}
