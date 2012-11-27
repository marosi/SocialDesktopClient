/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef PRIME_BUTTON_H_
#define PRIME_BUTTON_H_

#include <QWidget>
#include <QToolButton>

namespace Ui {
class PrimeButton;
}

namespace sdc {

class PrimeButton : public QWidget {

    Q_OBJECT

 public:
  explicit PrimeButton(QWidget *parent = 0);
  ~PrimeButton();

  QToolButton* button();

 private:
  Ui::PrimeButton *ui;
};

} // namespace sdc

#endif // PRIME_BUTTON_H_
