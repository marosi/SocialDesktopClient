/*
 * model.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 17, 2012
 */

#include "model.h"

#include "observer.h"
#include "boost/foreach.hpp"

namespace sdc {

void Model::RegisterObserver(ObserverRef observer) {
  observers_.push_back(observer);
}

void Model::NotifyObservers() const {
  for(unsigned int i = 0; i < observers_.size(); ++i) {
    observers_[i]->Update();
  }
  /*for (std::vector<ObserverRef>::const_iterator it = observers_.begin(); it
      <= observers_.end(); ++it) {
    (*it)->Update();
  }*/
  /*BOOST_FOREACH(const ObserverRef observer, observers_) {
    observer->Update();
  }*/
}

} /* namespace sdc */
