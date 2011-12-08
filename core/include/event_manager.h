/**
 * @file event_manager.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Nov 3, 2011
 */

#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include "common.h"
#include "abstract_manager.h"

#include <vector>
#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"

namespace sdc {

class EventManager : public AbstractManager {
 public:
  class Event;

  EventManager();

  void Run();

  void PostEvent(boost::function<void ()> callback);
  void PostMessage(Message* message);

  void Stop();

 private:
  void DoStop();
  void ProcessEvent(const Event& event);

  std::vector<Event> queue_;
  boost::mutex mutex_;
  boost::condition_variable is_empty_queue_;
  bool is_running_;
};

struct EventManager::Event {
  Event(boost::function<void ()> function) : callback(function) {}
  unsigned int id; //FIXME: this is unused so far
  boost::function<void ()> callback;
};

} //namespace sdc

#endif /* EVENT_MANAGER_H_ */
