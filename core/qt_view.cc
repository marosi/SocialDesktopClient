/*
 * qt_view.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jan 31, 2012
 */

#include "qt_view.h"

namespace sdc {

qtview::qtview(QWidget* parent) : QWidget(parent) {
  QObject::connect(this, SIGNAL(update()), this, SLOT(DoUpdate()));
}

void qtview::Update() {
  emit update();
}

} /* namespace sdc */
