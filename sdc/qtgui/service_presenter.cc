/*
 * service_presenter.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 6, 2012
 */

#include "service_presenter.h"
#include "qt_gui.h"

namespace sdc {

void ServicePresenter::SetStylesheet(const QString &stylesheet) {
  qtgui_->AppendStyleSheet(stylesheet);
}

} /* namespace sdc */
