#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "log.h"
#include "ui_mainwindow.h"
#include "view.h"
#include <QtGui/QMainWindow>
#include "boost/shared_ptr.hpp"

namespace sdc {

class MainWindow : public QMainWindow, public sdc::View
{
    Q_OBJECT

 public:
  MainWindow(QWidget *parent = 0) {
    ui_.setupUi(this);
  }

  ~MainWindow() {}

  void AddServiceWidget(QWidget* service) {
    ui_.contentPane->layout()->addWidget(service);
  }

  void SetSettingsView(QWidget* settings) {
    settings_ = settings;
    assert(settings_);
    connect(ui_.actionSettings, SIGNAL(triggered()),
            settings_, SLOT(show()));
  }

 public slots:

 private:
  Ui::MainWindowClass ui_;
  QWidget* settings_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
