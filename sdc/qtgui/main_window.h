#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_main_window.h"
#include <QtGui/QMainWindow>
#include <QList>
#include <QMultiMap>

namespace sdc {

class Account;
class AccountButton;
class ActivitiesButton;
class ActivitiesPanel;
class ContactWidget;
class ContactsButton;
class ContactsPanel;
class ContentPanel;
class MainButton;
class QtGui;
class QtServiceModel;
class ServicePresenter;

class MainWindow : public QMainWindow {

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

  ActivitiesPanel* activities() { return activities_; }

 private:
  template<class T>
  class GroupedBy;

  Ui::MainWindowClass ui;
  QtGui* qtgui_;
  QList<AccountButton*> buttons_;
  QMultiMap<ServicePresenter*, ContactWidget*> contacts_;
  QMultiMap<ServicePresenter*, ContentPanel*> contents_;
  GroupedBy<ServicePresenter*>* grouped_by_account_;
  MainButton* main_button_;
  ActivitiesButton* activities_button_;
  ActivitiesPanel* activities_;
  ContactsButton* contacts_button_;
  ContactsPanel* contacts_panel_;
};

} /* namespace sdc */

#endif // MAINWINDOW_H
