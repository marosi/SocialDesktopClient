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
#include "service_model.h"
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"

namespace sdc {

UI::UI(Core* core) : CoreAnchor(core) {}

UI::~UI() {}

void UI::Init() {}

} /* namespace sdc */
