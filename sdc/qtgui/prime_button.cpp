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

} /* namespace sdc */
