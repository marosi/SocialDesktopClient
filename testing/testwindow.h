#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_testwindow.h"

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    TestWindow(QWidget *parent = 0);
    ~TestWindow();

    Ui::TestWindowClass ui;
private:
    //Ui::TestWindowClass ui;
};

#endif // TESTWINDOW_H
