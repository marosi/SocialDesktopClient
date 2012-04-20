#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "account_data.h"
#include "account_button.h"
#include "log.h"
#include "ui_mainwindow.h"
#include "qt_view.h"
#include "qt_service_model.h"
#include "settings_dialog.h"
#include <QtGui/QMainWindow>
#include <QToolButton>
#include "boost/shared_ptr.hpp"

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

  void AddServiceWidget(QWidget* service) {
    service->setParent(this);
    ui.contentPane->layout()->addWidget(service);
  }



 public slots:
  void CreateSettingsView() {
    settings_ = new SettingsDialog(this);
  }

  void ActivateAccount(QtServiceModel* model) {
    AccountButton* button = model->GetQtService()->CreateAccountButton(this, model);
    ui.accountsPane->layout()->addWidget(button);
    buttons_ << button;
  }

  void DeactivateAccount(QtServiceModel* model) {
    for (int i = 0; i < buttons_.size(); ++i) {
      if (buttons_.at(i)->isThis(model)) {
        delete buttons_.at(i);
        buttons_.removeAt(i);
      }
    }
  }

 private:
  Ui::MainWindowClass ui;
  QWidget* settings_;
  QList<AccountButton*> buttons_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
