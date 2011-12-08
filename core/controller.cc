/*
 * controller.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Dec 6, 2011
 */

#include "controller.h"

#include <QString>

namespace sdc {

//void Controller::update(std::string content) {}

void Controller::SignalIt(std::string content) {
  QString* str = new QString(content.c_str());
  str->append("\n <br>");
  emit update(*str);
}

} /* namespace sdc */
