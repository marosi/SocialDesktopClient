/*
 * test_controller.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 16, 2012
 */

#include "test_controller.h"
#include "common.h"
#include "connection.h"
#include "message.h"
#include "connection_manager.h"
#include "core_typedefs.h"
#include <vector>

TestController::TestController() {}


void TestController::SendXmppStanza() {
  //GetModel()->SetText(GetView()->GetText());
  boost::shared_ptr<sdc::Message> message = boost::make_shared<sdc::Message>(GetView()->GetText());
  std::vector<sdc::ConnectionRef> conns;
  sdc::g_connection_manager->GetAllActiveConnections(conns);
  std::vector<sdc::ConnectionRef>::iterator it;
  for(it = conns.begin(); it != conns.end(); ++it) {
    sdc::g_event_manager->PostEvent(boost::bind(&sdc::Connection::SendMessage, (*it), message));
  }
  GetView()->ClearText();
}

void TestController::PrintMessageFromPlugin(boost::shared_ptr<sdc::Message> message) {
  GetModel()->SetText(message->GetText());
}
