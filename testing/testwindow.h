#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_testwindow.h"

#include <string>
#include "view.h"

//#include <QAction>

class TestModel;
class TestController;

class TestWindow : public QMainWindow, public sdc::GenericView<TestController, TestModel>
{
    Q_OBJECT

public:
    TestWindow(QWidget* parent=0);
    ~TestWindow();

    void Update();

    std::string GetText();
    void ClearText();

public slots:
    void OnConfirmText();
    void DoUpdate();

signals:
    void update();

private:
    Ui::TestWindowClass ui_;

    void CreateActions();
    //QAction* send_input_text_;
};

#endif // TESTWINDOW_H
