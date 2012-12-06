/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_EVENT_MANAGER_H_
#define CORE_EVENT_MANAGER_H_

#include "common.h"
#include "abstract_manager.h"
#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"
#include <vector>

namespace sdc {

class Core;
class Message;

/**
 * EventManager instance is responsible for the Core thread safety.
 *
 */
class EventManager : public AbstractManager {
 public:
  class Event;

  EventManager(Core* core) : AbstractManager(core), is_running_(true) {}

  /**
   * Execution point of event-loop prosessing.
   *
   * This method keeps thread running unless Stop() is called.
   */
  void Run();

  /**
   * Posts event callback to the queue.
   * @param callback
   */
  void PostEvent(boost::function<void ()> callback);

  /**
   * Stops executing event-loop.
   */
  void Stop();

 private:
  void DoStop();
  void ProcessEvent(const Event& event);

  std::vector<Event> queue_;
  boost::mutex mutex_;
  boost::condition_variable is_empty_queue_;
  bool is_running_;
};

/**
 * Simple structure encapsulating event metadata.
 */
struct EventManager::Event {
  Event(boost::function<void ()> function) : callback(function) {}
  unsigned int id;
  boost::function<void ()> callback;
};

}  // namespace sdc

#endif // CORE_EVENT_MANAGER_H_
