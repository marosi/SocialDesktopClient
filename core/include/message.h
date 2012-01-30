/**
 * @file message.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Nov 28, 2011
 */

#ifndef CORE_MESSAGE_H_
#define CORE_MESSAGE_H_

#include "common.h"
#include "event_manager.h"
#include "service.h"
#include <string>
#include <vector>

namespace sdc {

/*
class Command {
 public:
  virtual void Execute()=0;
 protected:
  void Assign() {
    g_event_manager->PostCommand(this);
  }
};

template<class Object>
class GenericCommand : public Command {
 public:
  typedef void (Object::* Method)();

  void PostponeExecutionOf(Method m, Object* o) {
    method_ = m;
    object_ = o;
  }
  virtual void Execute() {
    (object_->*method_)();
  }
 private:
  Method method_;
  Object* object_;
};
*/

/**
 * Basic core element carrying social network data.
 */
class Message {
 public:
  Message(const std::string &content) { text_ = content; }
  std::string GetText() { return text_; }
 private:
  std::string text_;
};

/*class DispatchMessageToCore : public Command {
 public:
  DispatchMessageToCore(Core* core, Message* msg) {
    core_ = core;
    msg_ = msg;
    Assign();
  }
  virtual void Execute() {
    core_->Process(msg_);
  }
 private:
  Core* core_;
  Message* msg_;
};

class DispatchMessageToConnections : public Command {
 public:
    DispatchMessageToConnections(Message* msg) {
      msg_ = msg;
    }
    virtual void Execute() {
      std::vector<Connection*> active_conns = g_connection_manager->GetAllActiveConnections();
      for(std::vector<Connection*>::iterator it = active_conss.begin(); it != active_conns.end(); ++it) {
        Connection* conn = *it;
        conn->
      }
    }
 private:
  Message* msg_;
};*/

/**
 * User should call this method for the message to be processed.
 */
class Dispatch {
 public:
  Dispatch() {}
};

class MessageDispatch : Dispatch {
 public:
  /*MessageDispatch(Message* message) {
    //g_event_manager->PostEvent(boost::bind(&Message::OnDispatch, message));
    //TODO: Domysliet ako to bude s objektom Dispatch a jeho hierarchiou
  }*/
};

class UiDispatch : Dispatch {
};

class PluginDispatch : Dispatch {

};

} /* namespace sdc */

#endif /* CORE_MESSAGE_H_ */
