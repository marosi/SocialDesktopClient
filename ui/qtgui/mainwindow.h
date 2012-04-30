#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "account_data.h"
#include "account_button.h"
#include "contact_widget.h"
#include "content_panel.h"
#include "log.h"
#include "new_contact_dialog.h"
#include "qt_view.h"
#include "qt_service_model.h"
#include "settings_dialog.h"
#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QSettings>
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"
#include "boost/cast.hpp"

namespace sdc {

class QtGui;

class MainWindow : public QMainWindow, public sdc::QtView
{
    Q_OBJECT

 public:
  MainWindow(QtGui* qtgui) : QtView(qtgui) { // setting Qt GUI layer object to the main View object
    ui.setupUi(this);

    ui.statusbar->hide();

    connect(ui.actionSettings, SIGNAL(triggered()),
        this, SLOT(ShowSettingsDialog()));

    connect(ui.addContactButton, SIGNAL(clicked()),
        this, SLOT(ShowNewContactDialog()));

    QSettings settings;
    //LOG(DEBUG) << "QSettings testing :: " << settings.value("another").toString().toStdString();

  }

  ~MainWindow() {
    QSettings settings;
    //settings.setValue("another", "buuuu");
  }

  void AddContact(ContactWidget* contact) {
    ui.contactPanelLayout->insertWidget(0, contact);
  }

  void AddContentPanel(ContentPanel* panel) {
    ui.contentFrame->layout()->addWidget(panel);
  }

 public slots:
  void ShowSettingsDialog() {
    settings_ = new SettingsDialog(this);
  }

  void ShowNewContactDialog() {
    NewContactDialog* dialog = new NewContactDialog(this);
    dialog->exec();
  }

  void ActivateAccount(QtServiceModel* model) {
    /*
     * Place account central button
     */
    AccountButton* button = model->GetQtService()->CreateAccountButton(this, model);
    QBoxLayout* lay = boost::polymorphic_downcast<QBoxLayout*>(ui.accountsPane->layout());
    lay->insertWidget(0, button);
    buttons_ << button;
  }

  void DeactivateAccount(AccountData* account) {
    /*
     * Remove account button
     */
    for (int i = 0; i < buttons_.size(); ++i) {// TODO: model does not exists here anymore, using it will lead to SEGFAULT, think of different approach of removing Account ICON
      if (buttons_.at(i)->isThis(account->GetServiceModel())) {  // TODO: change to fingerprint
        delete buttons_.at(i);
        buttons_.removeAt(i);
      }
    }
  }

 private:
  Ui::MainWindowClass ui;
  QWidget* settings_;
  QList<AccountButton*> buttons_;
  QList<ContactWidget*> contacts_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
