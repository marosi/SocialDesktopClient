/*
 * ui.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Mar 31, 2012
 */

#include "ui.h"
#include "connection.h"
#include "connection_manager.h"
#include "core.h"
#include "service_controller.h"
#include "service_model.h"
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"

namespace sdc {

UI::~UI() {

  BOOST_FOREACH (ServiceController* ctrl, controllers_) {
    delete ctrl;
  }

}

void UI::Init() {
  // Launch GUI of each initiated service connection

  BOOST_FOREACH (Service* service, core()->services()) {
    // TODO: Whaaat??
  }

}

} /* namespace sdc */
