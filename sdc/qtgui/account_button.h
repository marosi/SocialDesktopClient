#ifndef ACCOUNT_BUTTON_H
#define ACCOUNT_BUTTON_H

#include "prime_button.h"

namespace sdc {

class QtServiceModel;

class AccountButton : public PrimeButton {

    Q_OBJECT

 public:
  AccountButton(QtServiceModel* model);
  virtual ~AccountButton();

  QMenu* menu() {
    return menu_;
  }

 protected slots:
  void ShowMenu();
  void GoOnline();
  void GoOffline();

 protected:
  QMenu* menu_;
  QtServiceModel* model_;
};

} /* namespace sdc */

#endif // ACCOUNT_BUTTON_H
