/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef NEW_ACCOUNT_DIALOG_H_
#define NEW_ACCOUNT_DIALOG_H_

#include <QtGui/QDialog>
#include "ui_new_account_dialog.h"

namespace sdc {

class Account;
class NewAccountWidget;

class NewAccountDialog : public QDialog
{
    Q_OBJECT

 public:
  enum Action {
    NEW, EDIT
  };

  NewAccountDialog(QWidget *parent = 0);
  ~NewAccountDialog();

  /**
   * Fills fields of certain account based on index
   */
  void SetAccountForEditing(Account* account);

 public slots:
  void accept();

 private slots:
  void ChangeServicePane(int index);

 private:
  Action action_;
  Ui::NewAccountDialogClass ui;
  QMap<QString, int> combobox_service_index_;
  QMap<QString, NewAccountWidget*> service_panes_;
  NewAccountWidget* current_pane_;
  Account* account_;
};

} // namespace sdc

#endif // NEW_ACCOUNT_DIALOG_H_
