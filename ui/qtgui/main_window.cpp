/*
 * main_window.cpp
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 5, 2012
 */

#include "main_window.h"
#include "account_data.h"
#include "contact_widget.h"
#include "content_panel.h"
#include "log.h"
#include "new_contact_dialog.h"
#include "qt_service_model.h"
#include "service_presenter.h"
#include "settings_dialog.h"
#include <QToolButton>

namespace sdc {

MainWindow::MainWindow(QtGui* qtgui) :
    QtView(qtgui) {
  // setting Qt GUI layer object to the main View object
  ui.setupUi(this);
  ui.statusbar->hide();
  connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(ShowSettingsDialog()));
  connect(ui.addContactButton, SIGNAL(clicked()), this, SLOT(ShowNewContactDialog()));
}

MainWindow::~MainWindow() {}

void MainWindow::AddAccountButton(AccountButton* button) {
  ui.accountsPane->layout()->addWidget(button);
  buttons_ << button;
}

void MainWindow::RemoveAccountButton(AccountButton* button) {
  ui.accountsPane->layout()->removeWidget(button);
  buttons_.removeOne(button);
}

void MainWindow::AddContact(ServicePresenter* presenter, ContactWidget* contact) {
  std::string tooltip = qtgui()->GetService(presenter)->name() +
      " - " +
      qtgui()->GetModel(presenter)->account()->GetUid();
  contact->setToolTip(QString::fromStdString(tooltip));
  ui.contactPanelLayout->insertWidget(0, contact);
}

void MainWindow::AddContentPanel(ContentPanel* panel) {
  ui.contentFrame->layout()->addWidget(panel);
}

void MainWindow::ShowSettingsDialog() {
  settings_ = new SettingsDialog(this);
}

void MainWindow::ShowNewContactDialog() {
  NewContactDialog* dialog = new NewContactDialog(this);
  dialog->exec();
}

} /* namespace sdc */
