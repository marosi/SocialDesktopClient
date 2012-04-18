#include "settings_dialog.h"
#include "new_account_dialog.h"
#include "service.h"
#include "settings_dialog_controller.h"
#include <QMessageBox>
#include "boost/cast.hpp"
#include "boost/foreach.hpp"
#include "config_manager.h"

namespace sdc {

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent),
      QtView(parent) {
  // create controller
  new SettingsDialogController(this);
  // set up gui
	ui.setupUi(this);
	Prepare();
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::ChangePage(QListWidgetItem* current, QListWidgetItem* previous) {
  if (!current)
    current = previous;
  SettingsListItem* item = boost::polymorphic_downcast<SettingsListItem*>(current);
  ui.stackedWidget->setCurrentWidget(item->GetPage());
}

void SettingsDialog::RefreshAccountsTable() {
  std::vector<AccountData*> accdata = core()->data()->GetAccounts();
  ui.accountsTableWidget->setRowCount(accdata.size());
  QStringList header;
  header.append("Account name");
  header.append("Enabled");
  ui.accountsTableWidget->setHorizontalHeaderLabels(header);
  ui.accountsTableWidget->setColumnWidth(1, 50);
  ui.accountsTableWidget->setColumnWidth(0, 280);
  for (unsigned int i = 0; i < accdata.size(); ++i) {
    //ui.accountsTableWidget->setCellWidget(i, 0, new QLabel(QString::fromStdString(ad.GetUid()), this));
    QTableWidgetItem* it = new QTableWidgetItem(QString::fromStdString(accdata[i]->GetUid()));
    it->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui.accountsTableWidget->setItem(i, 0, it);
    // checkbox
    it = new QTableWidgetItem;
    it->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    if (accdata[i]->IsEnabled()) {
      it->setCheckState(Qt::Checked);
    } else {
      it->setCheckState(Qt::Unchecked);
    }
    it->setTextAlignment(Qt::AlignCenter);
    ui.accountsTableWidget->setItem(i, 1, it);
  }
}

void SettingsDialog::ManageAccountsTableDataChanged(QTableWidgetItem* item) {
  if (ui.accountsTableWidget->currentColumn() != 1)
    return;
  int id = ui.accountsTableWidget->currentRow();
  if (item->checkState() == Qt::Checked) {
    core()->data()->SetEnabledAccount(id, true);
  } else if (item->checkState() == Qt::Unchecked) {
    core()->data()->SetEnabledAccount(id, false);
  }
}

void SettingsDialog::AccountSelected() {
  ui.deleteAccountButton->show();
  ui.editAccountButton->show();
}

void SettingsDialog::DeleteAccount() {
  int account = ui.accountsTableWidget->currentRow();
  LOG(DEBUG) << "current row : " << account;
  QMessageBox confirm;
  confirm.setText("Are you sure you want to delete selected account?");
  confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  confirm.setDefaultButton(QMessageBox::No);
  int result = confirm.exec();
  if (result == QMessageBox::Yes) {
    core()->data()->RemoveAccount(account);
  }
}

void SettingsDialog::EditAccount() {
  NewAccountDialog* dialog = new NewAccountDialog(this);
  AccountData* account = core()->data()->GetAccount(ui.accountsTableWidget->currentRow());
  dialog->SetAccountForEditing(account);
}

void SettingsDialog::ShowNewAccountDialog() {
  new NewAccountDialog(this);
}

void SettingsDialog::Prepare() {
  SettingsListItem* it1 = new SettingsListItem(ui.listWidget, ui.accountsPage);
  it1->setText("Accounts");

  RefreshAccountsTable();
  connect(ui.addAccountButton, SIGNAL(clicked()),
      this, SLOT(ShowNewAccountDialog()));
  connect(this->qtgui(), SIGNAL(accountsChanged()),
      this, SLOT(RefreshAccountsTable()));
  connect(ui.accountsTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)),
      this, SLOT(ManageAccountsTableDataChanged(QTableWidgetItem*)));
  connect(ui.accountsTableWidget, SIGNAL(itemSelectionChanged()),
      this, SLOT(AccountSelected()));

  ui.deleteAccountButton->hide();
  ui.editAccountButton->hide();

  connect(ui.deleteAccountButton, SIGNAL(clicked()),
      this, SLOT(DeleteAccount()));
  connect(ui.editAccountButton, SIGNAL(clicked()),
      this, SLOT(EditAccount()));

  SettingsListItem* it2 = new SettingsListItem(ui.listWidget, ui.pluginsPage);
  it2->setText("Plugins");

  connect(ui.listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
      this, SLOT(ChangePage(QListWidgetItem*, QListWidgetItem*)));
}

} /* namespace sdc */
