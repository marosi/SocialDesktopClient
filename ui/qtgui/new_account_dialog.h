#ifndef NEW_ACCOUNT_WIDGET_H
#define NEW_ACCOUNT_WIDGET_H

#include "qt_view.h"
#include <QtGui/QDialog>
#include "ui_new_account_dialog.h"

namespace sdc {

class NewAccountWidget;

class NewAccountDialog : public QDialog, public QtView
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
  void SetAccountForEditing(AccountData* account);

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
  AccountData* account_;
};

} /* namespace sdc */

#endif // NEW_ACCOUNT_WIDGET_H
