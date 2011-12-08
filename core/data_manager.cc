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

DataManager::DataManager() : controller_(new Controller) {}

void DataManager::AddContent(std::string content) {
  main_content_.push_back(content);
  controller_->SignalIt(content);
}

void DataManager::ConnectView() {
  TestWindow* tw = (TestWindow*) view_;
  QObject::connect(controller_, SIGNAL(update(QString)),
      tw->ui.textBrowser, SLOT(insertHtml(QString)));
}

void DataManager::SetView(void* view) {
  view_ = view;
}

} /* namespace sdc */
