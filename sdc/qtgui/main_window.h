#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qt_view.h"
#include "ui_main_window.h"
#include <QtGui/QMainWindow>
#include <QList>
#include <QMultiMap>

namespace sdc {

class Account;
class AccountButton;
class ActivityPanel;
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

  void AddContact(ServicePresenter* parent, ContactWidget* contact);
  void RemoveAllContacts(ServicePresenter* parent);

  void AddContentPanel(ServicePresenter* parent, ContentPanel* panel);
  void RemoveAllContentPanels(ServicePresenter* parent);

  ActivityPanel* activities() { return activities_; }

 public slots:
  void ShowSettingsDialog();
  void ShowNewContactDialog();

 private:
  template<class T>
  class GroupedBy;

  Ui::MainWindowClass ui;
  QWidget* settings_;
  QList<AccountButton*> buttons_;
  QMultiMap<ServicePresenter*, ContactWidget*> contacts_;
  QMultiMap<ServicePresenter*, ContentPanel*> contents_;
  GroupedBy<ServicePresenter*>* grouped_by_account_;
  ActivityPanel* activities_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
