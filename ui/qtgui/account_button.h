#ifndef ACCOUNT_BUTTON_H
#define ACCOUNT_BUTTON_H

#include "qt_view.h"
#include <QtGui/QWidget>
#include "ui_account_button.h"

namespace sdc {

class ServiceModel;

class AccountButton : public QWidget, public QtView {

    Q_OBJECT

 public:
  AccountButton(QWidget *parent, QtServiceModel* model);
  virtual ~AccountButton();

  bool isThis(ServiceModel* model) {
    return model == model_;
  }

  QMenu* menu() {
    return menu_;
  }

 protected slots:
  void ShowMenu();
  void GoOnline();
  void GoOffline();

 protected:
  Ui::AccountButtonClass ui;
  QMenu* menu_;

  ServiceModel* model_; // TODO: Temporary, change to some other account fingerprint
};

} /* namespace sdc */

#endif // ACCOUNT_BUTTON_H
