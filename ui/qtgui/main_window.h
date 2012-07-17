#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qt_view.h"
#include "ui_mainwindow.h"
#include <QtGui/QMainWindow>
#include <QList>

namespace sdc {

class AccountData;
class AccountButton;
class ContactWidget;
class ContentPanel;
class QtGui;
class QtServiceModel;

class MainWindow : public QMainWindow, public sdc::QtView {

    Q_OBJECT

 public:
  MainWindow(QtGui* qtgui);
  ~MainWindow();
  void AddAccountButton(AccountButton* button);
  void RemoveAccountButton(AccountButton* button);
  void AddContact(ServicePresenter* presenter, ContactWidget* contact);
  void AddContentPanel(ContentPanel* panel);

 public slots:
  void ShowSettingsDialog();
  void ShowNewContactDialog();

 private:
  Ui::MainWindowClass ui;
  QWidget* settings_;
  QList<AccountButton*> buttons_;
  QList<ContactWidget*> contacts_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
