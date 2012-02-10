/*
 * b_controller.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 6, 2012
 */

#include "b_controller.h"
#include "buddycloud_connection.h"

using std::string;

BController::BController() {
  ConnectView();
}

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

void BController::ConnectView() {

}
