/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "event_manager.h"
#include "log.h"
#include "boost/foreach.hpp"

namespace sdc {

void EventManager::PostEvent(boost::function<void ()> callback) {
  boost::unique_lock<boost::mutex> lock(mutex_);
  Event event(callback);
  queue_.push_back(event);
  is_empty_queue_.notify_one();
}

void EventManager::Run() {
  // Run, Forest, run!
  while (is_running_) {
    std::vector<Event> events_to_process;
    { // lock the code block so we can access queue
      boost::unique_lock<boost::mutex> lock(mutex_);
      if (queue_.empty()) {
        // block core event loop until new event enters the queue
        is_empty_queue_.wait(lock);
      } else {
        // take queued events for processing and reset queue
        queue_.swap(events_to_process);
      }
    }
    // Log number of taken events, for
    LOG(DEBUG3) << "Events taken: " << events_to_process.size();
    BOOST_FOREACH (const Event& event , events_to_process)
    {
      ProcessEvent(event);
    }
  }
}

void EventManager::ProcessEvent(const Event& event) {
  try {
    event.callback();
  } catch (const std::exception& e) {
    LOG(ERROR) << "Uncaught exception while processing event with id: " << event.id << std::endl
        << e.what();
  }
}

/**
 * In order to stop EventManager correctly all before added events in the queue has to be processed.
 * Therefore also stop event needs to be posted to the queue.
 */
void EventManager::Stop() {
  PostEvent(boost::bind(&EventManager::DoStop, this));
}

void EventManager::DoStop() {
  is_running_ = false;
}

} // namespace sdc
