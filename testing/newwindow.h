#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_newwindow.h"

class NewWindow : public QMainWindow
{
    Q_OBJECT

public:
    NewWindow(QWidget *parent = 0);
    ~NewWindow();

private:
    Ui::NewWindowClass ui;
};

#endif // NEWWINDOW_H
