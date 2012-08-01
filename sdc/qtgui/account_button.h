#ifndef ACCOUNT_BUTTON_H
#define ACCOUNT_BUTTON_H

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

} /* namespace sdc */

#endif // ACCOUNT_BUTTON_H
