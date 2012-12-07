/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BC_NEW_ACCOUNT_WIDGET_H_
#define BC_NEW_ACCOUNT_WIDGET_H_

#include "bc_account.h"
#include "sdc/qtgui/new_account_widget.h"
#include <QtGui/QWidget>
#include "ui_bc_new_account_widget.h"

/**
 * buddycloud new account widget implementation.
 */
class BcNewAccountWidget : public sdc::NewAccountWidget {

    Q_OBJECT

 public:
  BcNewAccountWidget(QWidget* /*parent*/ = 0) {
    ui.setupUi(this);
  }
  ~BcNewAccountWidget() {}

  void BindDataTo(sdc::Account* account) {
    account->SetUid(ui.usernameLineEdit->text().toStdString());
    account->SetPassword(ui.passwordLineEdit->text().toStdString());
  }

  void BindDataFrom(const sdc::Account* account) {
    ui.usernameLineEdit->setText(QString::fromStdString(account->GetUid()));
    ui.passwordLineEdit->setText(QString::fromStdString(account->GetPassword()));
  }

  sdc::Account* CreateNew() {
    BcAccount* account = new BcAccount;
    this->BindDataTo(account);
    return account;
  }

  bool IsDataValid() {
    if (ui.usernameLineEdit->text() == "" || ui.passwordLineEdit->text() == "") {
      this->SetErrorMessage("Some of the fields are empty!");
      return false;
    }
    return true;
  }

 private:
    Ui::BcNewAccountWidgetClass ui;
};

#endif // BC_NEW_ACCOUNT_WIDGET_H_
