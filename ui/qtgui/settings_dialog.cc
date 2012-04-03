#include "settings_dialog.h"
#include "boost/cast.hpp"

namespace sdc {

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
	ui.setupUi(this);
	PrepareSettingsDialog();
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::ChangePage(QListWidgetItem* current, QListWidgetItem* previous) {
  SettingsListItem* item = boost::polymorphic_downcast<SettingsListItem*>(current);
  ui.stackedWidget->setCurrentWidget(item->GetPage());
}

void SettingsDialog::PrepareSettingsDialog() {
  SettingsListItem* it1 = new SettingsListItem(ui.listWidget, ui.accountsPage);
  it1->setText("Accounts");

  SettingsListItem* it2 = new SettingsListItem(ui.listWidget, ui.pluginsPage);
  it2->setText("Plugins");

  QListWidgetItem* it = new QListWidgetItem("cuuuz", ui.listWidget);
  it = new QListWidgetItem("cuuuz", ui.listWidget);
  it = new QListWidgetItem("cuuuz", ui.listWidget);
  it = new QListWidgetItem("cuuuz", ui.listWidget);
  it = new QListWidgetItem("cuuuz", ui.listWidget);

  connect(ui.listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
      this, SLOT(ChangePage(QListWidgetItem*, QListWidgetItem*)));
}

} /* namespace sdc */
