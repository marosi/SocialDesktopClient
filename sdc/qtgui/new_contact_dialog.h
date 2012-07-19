#ifndef NEW_CONTACT_DIALOG_H
#define NEW_CONTACT_DIALOG_H

#include "qt_view.h"
#include <QtGui/QDialog>
#include "ui_new_contact_dialog.h"

namespace sdc {

class NewContactWidget;

class NewContactDialog : public QDialog, public QtView {

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

} /* namespace sdc */

#endif // NEW_CONTACT_DIALOG_H
