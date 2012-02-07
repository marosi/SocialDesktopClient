#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "main_view.h"
#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "boost/shared_ptr.hpp"

class MainWindow : public QMainWindow, public sdc::MainView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void StartUpServiceMainWidget(boost::shared_ptr<QWidget> service_main_view) {
      //service_main_view->show();
      service_views_.push_back(service_main_view);
    }

private:
    Ui::MainWindowClass ui;
    std::vector<boost::shared_ptr<QWidget> > service_views_;
};

#endif // MAINWINDOW_H
