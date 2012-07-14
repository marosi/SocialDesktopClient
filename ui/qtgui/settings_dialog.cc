#include "core.h"
#include "settings_dialog.h"
#include "new_account_dialog.h"
#include "service.h"
#include "accounts_model.h"
#include <QMessageBox>
#include "boost/cast.hpp"
#include "boost/foreach.hpp"
#include "config_manager.h"

namespace sdc {

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent),
      QtView(parent) {
  // set up gui
	ui.setupUi(this);
  // setup accounts page
  SettingsListItem* it1 = new SettingsListItem(ui.listWidget, ui.accountsPage);
  it1->setText("Accounts");

  AccountsModel* model = new AccountsModel(core());
  ui.tableView->setModel(model);
  ui.tableView->setColumnWidth(1, 50);
  ui.tableView->setColumnWidth(0, 280);

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

	show();
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
    core()->data()->RemoveAccount(index);
  }
}

void SettingsDialog::EditAccount() {
  NewAccountDialog* dialog = new NewAccountDialog(this);
  int index = ui.tableView->selectionModel()->selectedIndexes().first().row();
  AccountData* account = core()->data()->GetAccount(index);
  dialog->SetAccountForEditing(account);
}

void SettingsDialog::ShowNewAccountDialog() {
  new NewAccountDialog(this);
}

} /* namespace sdc */
