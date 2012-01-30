/*
 * test_controller.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 16, 2012
 */

#include "test_controller.h"
#include "common.h"
#include "service.h"
#include "message.h"
#include "connection_manager.h"
#include <vector>

TestController::TestController(boost::shared_ptr<TestModel> model) :
    sdc::GenericController<TestModel, TestWindow, TestController>(model)
{
  LOG(DEBUG3) << "##################### TestController CONSTRUCTOR";
}


void TestController::SendXmppStanza() {
  //GetModel()->SetText(GetView()->GetText());
  boost::shared_ptr<sdc::Message> message = boost::make_shared<sdc::Message>(GetView()->GetText());
  std::vector<sdc::Service::Connection*> conns = sdc::g_connection_manager->GetAllActiveConnections();
  std::vector<sdc::Service::Connection*>::iterator it;
  for(it = conns.begin(); it != conns.end(); ++it) {
    sdc::g_event_manager->PostEvent(boost::bind(&sdc::Service::Connection::SendMessage, (*it), message));
  }
}

void TestController::PrintMessageFromPlugin(boost::shared_ptr<sdc::Message> message) {
  GetModel()->SetText(message->GetText());
}

void TestController::Init() {
  sdc::GenericController<TestModel, TestWindow, TestController>::Init();
  GetView()->show();
  LOG(DEBUG3) << "##################### show()";
}
