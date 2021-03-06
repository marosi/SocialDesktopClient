/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "core.h"
#include "data_manager.h"
#include "settings_dialog.h"
#include "new_account_dialog.h"
#include "service.h"
#include "accounts_model.h"
#include <QMessageBox>
#include "boost/cast.hpp"

namespace sdc {

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
  // set up gui
	ui.setupUi(this);
  // setup accounts page
  SettingsListItem* it1 = new SettingsListItem(ui.listWidget, ui.accountsPage);

  QLabel* accounts = new QLabel;
  accounts->setPixmap(QPixmap(":/icons/accounts.png"));
  accounts->setScaledContents(true);
  accounts->setMaximumWidth(70);
  accounts->setMaximumHeight(70);
  it1->setSizeHint(QSize(70, 70));
  ui.listWidget->setItemWidget(it1, accounts);

  // setup accounts model and view
  AccountsModel* model = new AccountsModel(Core::Instance());
  ui.tableView->setModel(model);
  ui.tableView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);

  connect(ui.addAccountButton, SIGNAL(clicked()),
      this, SLOT(ShowNewAccountDialog()));
  connect(ui.tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
      this, SLOT(AccountSelected()));

  ui.deleteAccountButton->hide();
  ui.editAccountButton->hide();

  connect(ui.deleteAccountButton, SIGNAL(clicked()),
      this, SLOT(DeleteAccount()));
  connect(ui.editAccountButton, SIGNAL(clicked()),
      this, SLOT(EditAccount()));

  /*SettingsListItem* it2 = new SettingsListItem(ui.listWidget, ui.pluginsPage);
  it2->setText("Plugins");*/

  connect(ui.listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
      this, SLOT(ChangePage(QListWidgetItem*, QListWidgetItem*)));

  // dialog OK and Cancel buttons
  connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::ChangePage(QListWidgetItem* current, QListWidgetItem* previous) {
  if (!current)
    current = previous;
  SettingsListItem* item = boost::polymorphic_downcast<SettingsListItem*>(current);
  ui.stackedWidget->setCurrentWidget(item->GetPage());
}

void SettingsDialog::AccountSelected() {
  ui.deleteAccountButton->show();
  ui.editAccountButton->show();
}

void SettingsDialog::DeleteAccount() {
  int index = ui.tableView->selectionModel()->selectedIndexes().first().row();
  QMessageBox confirm;
  confirm.setText("Are you sure you want to delete selected account?");
  confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  confirm.setDefaultButton(QMessageBox::No);
  int result = confirm.exec();
  if (result == QMessageBox::Yes) {
    Core::Instance()->data()->RemoveAccount(index);
  }
}

void SettingsDialog::EditAccount() {
  NewAccountDialog* dialog = new NewAccountDialog(this);
  int index = ui.tableView->selectionModel()->selectedIndexes().first().row();
  Account* account = Core::Instance()->data()->GetAccount(index);
  dialog->SetAccountForEditing(account);
}

void SettingsDialog::ShowNewAccountDialog() {
  new NewAccountDialog(this);
}

} // namespace sdc
