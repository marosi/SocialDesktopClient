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
  connect(this, SIGNAL(acknowledgeOnlineState(const QString &)),
      channel_view_, SLOT(ShowState(const QString &)));
}

void BController::AcknowledgeOnlineState() {
  model_->SetOnlineState(state_to_be_acknowledged_);
  QString str = QString::fromStdString(model_->GetOnlineState());
  LOG(DEBUG) << str.toStdString();
  emit acknowledgeOnlineState(str);
}

void BController::SwitchOnlineState(const QString &state) {
  QString lower = state.toLower();
  if (lower == "online") {
    GetConnection<BuddycloudConnection>()->Connect();
    channel_view_->ShowConnecting();;
  } else if (lower == "offline" && GetModel<BModel>()->GetOnlineState() != "offline") {
    GetConnection<BuddycloudConnection>()->Disconnect();
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
