/*
 * data_manager.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Dec 6, 2011
 */

#include "data_manager.h"

#include <QObject>
#include <QString>
#include "testing/testwindow.h"
#include "controller.h"

namespace sdc {

DataManager::DataManager() {}

void DataManager::AddContent(std::string content) {
  main_content_.push_back(content);
}

} /* namespace sdc */
