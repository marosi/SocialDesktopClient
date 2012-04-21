#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "account_data.h"
#include "account_button.h"
#include "contact_widget.h"
#include "log.h"
#include "qt_view.h"
#include "qt_service_model.h"
#include "settings_dialog.h"
#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include <QToolButton>
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"

namespace sdc {

class QtGui;

class MainWindow : public QMainWindow, public sdc::QtView
{
    Q_OBJECT

 public:
  MainWindow(QtGui* qtgui) : QtView(qtgui) { // setting Qt GUI layer object to the main View object
    ui.setupUi(this);

    connect(ui.actionSettings, SIGNAL(triggered()),
        this, SLOT(CreateSettingsView()));
  }

  ~MainWindow() {}

 public slots:
  void CreateSettingsView() {
    settings_ = new SettingsDialog(this);
  }

  void ActivateAccount(QtServiceModel* model) {
    /*
     * Place account central button
     */
    AccountButton* button = model->GetQtService()->CreateAccountButton(this, model);
    ui.accountsPane->layout()->addWidget(button);
    buttons_ << button;
    /*
     * Load contacts
     */
    LoadContacts(model);
    connect(model, SIGNAL(contactsChanged()),
        this, SLOT(OnContactsChanged()));
  }

  void DeactivateAccount(QtServiceModel* model) {
    /*
     * Remove account button
     */
    for (int i = 0; i < buttons_.size(); ++i) {
      if (buttons_.at(i)->isThis(model)) {
        delete buttons_.at(i);
        buttons_.removeAt(i);
      }
    }
    /*
     * Remove contacts
     */
    ClearContacts(model);
  }

  void OnContactsChanged() {
    /*
     * Clear account contacts from common panel
     */
    QtServiceModel* model = boost::polymorphic_cast<QtServiceModel*>(sender());
    ClearContacts(model);
    LoadContacts(model);
  }

 private:
  void ClearContacts(QtServiceModel* fingerprint) {
    LOG(DEBUG) << contacts_.size();
    QList<ContactWidget*>::iterator it = contacts_.begin();
    for (; it != contacts_.end(); ++it) {
      LOG(DEBUG) << "ITERATING";
      if ((*it)->IsMatching(fingerprint)) {
        LOG(DEBUG) << "Removing contact!!!!";
        delete (*it);
        contacts_.erase(it);
      }
    }
  }

  void LoadContacts(QtServiceModel* model) {
    BOOST_FOREACH (Contact::Ref contact, model->GetContacts()) {
      ContactWidget* cw = model->GetQtService()->CreateContactWidget(this, contact);
      cw->SetFingerprint(model);
      ui.contactPanelLayout->insertWidget(0, cw);
      contacts_ << cw;
    }
  }

  Ui::MainWindowClass ui;
  QWidget* settings_;
  QList<AccountButton*> buttons_;
  QList<ContactWidget*> contacts_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
