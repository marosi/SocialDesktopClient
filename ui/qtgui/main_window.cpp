/*
 * main_window.cpp
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 5, 2012
 */

#include "main_window.h"
#include "account_data.h"
#include "account_button.h"
#include "contact_widget.h"
#include "content_panel.h"
#include "log.h"
#include "new_contact_dialog.h"
#include "qt_service_model.h"
#include "service_presenter.h"
#include "settings_dialog.h"
#include <QToolButton>
#include <QSettings>
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"
#include "boost/cast.hpp"

namespace sdc {

MainWindow::MainWindow(QtGui* qtgui) :
    QtView(qtgui) {
  // setting Qt GUI layer object to the main View object
  ui.setupUi(this);
  ui.statusbar->hide();
  connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(ShowSettingsDialog()));
  connect(ui.addContactButton, SIGNAL(clicked()), this, SLOT(ShowNewContactDialog()));
  QSettings settings;
  //LOG(DEBUG) << "QSettings testing :: " << settings.value("another").toString().toStdString();
}

MainWindow::~MainWindow() {
  QSettings settings;
  //settings.setValue("another", "buuuu");
}

void MainWindow::AddContact(ServicePresenter* presenter, ContactWidget* contact) {
  std::string tooltip = qtgui()->GetService(presenter)->name() +
      " - " +
      qtgui()->GetModel(presenter)->GetAccount()->GetUid();
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

void MainWindow::ActivateAccount(QtServiceModel* model) {
  /*
   * Place account central button
   */
  AccountButton* button = new AccountButton(this, model);
  QBoxLayout* lay = boost::polymorphic_downcast<QBoxLayout*>(ui.accountsPane->layout());
  lay->insertWidget(0, button);
  buttons_ << button;

  qtgui()->GetPresenter(model)->account_button_ = button;
  qtgui()->GetPresenter(model)->Init();
}

void MainWindow::DeactivateAccount(AccountData* account) {
  /*
   * Remove account button
   */
//  for (int i = 0; i < buttons_.size(); ++i) {// TODO: model does not exists here anymore, using it will lead to SEGFAULT, think of different approach of removing Account ICON
//    if (buttons_.at(i)->isThis(account->GetServiceModel())) {  // TODO: change to fingerprint
//      delete buttons_.at(i);
//      buttons_.removeAt(i);
//    }
//  }
}

} /* namespace sdc */
