/*
 * simple_message.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Dec 6, 2011
 */

#include "simple_message.h"

#include "common.h"
#include "data_manager.h"

namespace sdc {

SimpleStringMessage::SimpleStringMessage(std::string message) {
  content_ = message;
}

void SimpleStringMessage::Process() {
  g_data_manager->AddContent(content_);
}

} /* namespace sdc */
