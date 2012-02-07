/**
 * @file test_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 9, 2012
 */

#ifndef TEST_CONTROLLER_H_
#define TEST_CONTROLLER_H_

#include "controller.h"

#include "test_model.h"
#include "testwindow.h"
#include "message.h"

class TestController : public sdc::GenericController<TestModel, TestWindow> {
 public:
  TestController();
  void SendXmppStanza();
  void PrintMessageFromPlugin(boost::shared_ptr<sdc::Message> message);
};

#endif /* TEST_CONTROLLER_H_ */
