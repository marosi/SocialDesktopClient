#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "log.h"
#include "ui_mainwindow.h"
#include "qt_view.h"
#include "settings_dialog.h"
#include <QtGui/QMainWindow>
#include "boost/shared_ptr.hpp"

namespace sdc {

class QtGui;

class MainWindow : public QMainWindow, public sdc::QtView
{
    Q_OBJECT

 public:
  MainWindow(QtGui* qtgui) : QtView(qtgui) { // setting Qt GUI layer object to the main View object
    ui_.setupUi(this);

    connect(ui_.actionSettings, SIGNAL(triggered()),
        this, SLOT(CreateSettingsView()));
  }

  ~MainWindow() {}

  void AddServiceWidget(QWidget* service) {
    service->setParent(this);
    ui_.contentPane->layout()->addWidget(service);
  }

 public slots:
  void CreateSettingsView() {
    settings_ = new SettingsDialog(this);
  }

 private:
  Ui::MainWindowClass ui_;
  QWidget* settings_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
