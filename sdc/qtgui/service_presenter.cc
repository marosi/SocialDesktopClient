/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "service_presenter.h"
#include "qt_gui.h"

namespace sdc {

ServicePresenter::ServicePresenter() {}

void ServicePresenter::SetStylesheet(const QString &stylesheet) {
  qtgui_->AppendStyleSheet(stylesheet);
}

} // namespace sdc
