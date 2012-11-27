/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "prime_button.h"
#include "ui_prime_button.h"

namespace sdc {

PrimeButton::PrimeButton(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PrimeButton) {
  ui->setupUi(this);
}

PrimeButton::~PrimeButton() {
  delete ui;
}

QToolButton* PrimeButton::button() {
  return ui->toolButton;
}

} // namespace sdc
