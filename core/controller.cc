/*
 * controller.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Dec 6, 2011
 */

#include "controller.h"

#include "model.h"
#include "view.h"
#include <QString>


namespace sdc {

/*GenericController::GenericController(Model model, View view) :
      Controller(model, view),
      generic_model_(model),
      generic_view_(view) {}


TestController::TestController(DataManager* model, TestWindow* view) :
    GenericController(model, view),
    data_manager_(model),
    test_window_(view)
{
  Init();
}

void TestController::Update(std::string content) {
  QString* str = new QString(content.c_str());
  str->append("\n <br>");
  emit update(*str);
}

//void Controller::update(std::string content) {}

void TestController::Init() {
  QObject::connect(this, SIGNAL(update(QString)), test_window_->ui.textBrowser,
      SLOT(insertHtml(QString)));
}*/

} /* namespace sdc */
