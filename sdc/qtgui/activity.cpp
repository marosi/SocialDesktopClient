/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "activity.h"
#include "ui_activity.h"

namespace sdc {

Activity::Activity(QFrame *parent)
  : QFrame(parent),
    ui(new Ui::Activity) {
  ui->setupUi(this);
}

Activity::~Activity() {
  delete ui;
}

QLabel* Activity::image_label() {
  return ui->imageLabel;
}

QLabel* Activity::text_label() {
  return ui->textLabel;
}

} // namespace sdc
