#ifndef NEW_ACCOUNT_WIDGET_H
#define NEW_ACCOUNT_WIDGET_H

#include "sdc.h"
#include "bc_account.h"
#include <QtGui/QWidget>
#include "ui_bc_new_account_widget.h"

class BcNewAccountWidget : public sdc::NewAccountWidget
{
    Q_OBJECT

 public:
  BcNewAccountWidget(QWidget *parent = 0) {
    ui.setupUi(this);
  }
  ~BcNewAccountWidget() {}

  void BindDataTo(sdc::AccountData* account) {
    account->SetUid(ui.usernameLineEdit->text().toStdString());
    account->SetPassword(ui.passwordLineEdit->text().toStdString());
  }

  void BindDataFrom(const sdc::AccountData* account) {
    ui.usernameLineEdit->setText(QString::fromStdString(account->GetUid()));
    ui.passwordLineEdit->setText(QString::fromStdString(account->GetPassword()));
  }

  sdc::AccountData* CreateNew() {
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

#endif // NEW_ACCOUNT_WIDGET_H
