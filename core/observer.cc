/*
 * observer.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 17, 2012
 */

#include "observer.h"

#include "model.h"

namespace sdc {

Observer::~Observer() {}

void Observer::SetModel(boost::shared_ptr<Model> model) {
  model_ = model;
  model_->RegisterObserver(this->shared_from_this());
}

} /* namespace sdc */
