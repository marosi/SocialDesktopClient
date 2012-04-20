#ifndef ACCOUNT_BUTTON_H
#define ACCOUNT_BUTTON_H

#include "qt_view.h"
#include <QtGui/QWidget>
#include "ui_account_button.h"

namespace sdc {

class QtServiceModel;

class AccountButton : public QWidget, public QtView {
    Q_OBJECT

 public:
  AccountButton(QWidget *parent, QtServiceModel* model);
  ~AccountButton();

  bool isThis(QtServiceModel* model) {
    return model == model_;
  }

 protected slots:
  void ShowMenu();
  void GoOnline();
  void GoOffline();

 protected:
  Ui::AccountButtonClass ui;
  QMenu* menu_;

  QtServiceModel* model_;
};

} /* namespace sdc */

#endif // ACCOUNT_BUTTON_H
