/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef NEW_CONTACT_DIALOG_H_
#define NEW_CONTACT_DIALOG_H_

#include <QtGui/QDialog>
#include "ui_new_contact_dialog.h"

namespace sdc {

class NewContactWidget;

class NewContactDialog : public QDialog {

    Q_OBJECT

 public:
  NewContactDialog(QWidget *parent);
  ~NewContactDialog();

 public slots:
  void accept();

 private slots:
  void ChangeServicePane(int index);

 private:
  Ui::NewContactDialogClass ui;
  NewContactWidget* contact_pane_;
};

} // namespace sdc

#endif // NEW_CONTACT_DIALOG_H_
