/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ACCOUNT_BUTTON_H_
#define ACCOUNT_BUTTON_H_

#include "prime_button.h"

class QIcon;
class QAction;

namespace sdc {

class QtServiceModel;

class AccountButton : public PrimeButton {

    Q_OBJECT

 public:
  AccountButton(QtServiceModel* model);
  virtual ~AccountButton();

  void SetAccountIcon(const QIcon &icon);

  QAction* title_action() { return title_; }

 protected slots:
  void GoOnline();
  void GoOffline();

 private:
  QtServiceModel* model_;
  QMenu* menu_;
  QAction* title_;
};

} // namespace sdc

#endif // ACCOUNT_BUTTON_H_
