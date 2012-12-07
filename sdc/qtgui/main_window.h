/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include "ui_main_window.h"
#include <QtGui/QMainWindow>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QToolButton>

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

/**
 * A main application window class.
 */
class MainWindow : public QMainWindow {

    Q_OBJECT

 public:
  MainWindow(QtGui* qtgui);
  ~MainWindow();

  /**
   * Interface for handling account prime buttons within the main window control panel.
   * @see PrimeButton
   */
  void AddAccountButton(AccountButton* button);
  void RemoveAccountButton(AccountButton* button);

  /**
   * Interface for handling general contacts.
   */
  void AddContact(ServicePresenter* parent, ContactWidget* contact);
  void RemoveAllContacts(ServicePresenter* parent);

  /**
   * Interface for adding/removing service buttons inside the header of contacts panel.
   */
  void AddContactsButton(ServicePresenter* parent, QToolButton* btn);
  void RemoveContactsButton(ServicePresenter* parent);

  /**
   * Interface for handling content panels withing the main window content area.
   */
  void AddContentPanel(ServicePresenter* parent, ContentPanel* panel);
  void RemoveAllContentPanels(ServicePresenter* parent);

  /**
   * Gets common activities panel.
   * @return activities panel
   */
  ActivitiesPanel* activities() { return activities_; }

  /**
   * Gets common contacts panel.
   * @return contacts panel
   */
  ContactsPanel* contacts() { return contacts_panel_; }

 private:
  template<class T>
  class GroupedBy;

  Ui::MainWindowClass ui;
  QtGui* qtgui_;
  QList<AccountButton*> buttons_;
  QMultiMap<ServicePresenter*, ContactWidget*> contacts_;
  QMultiMap<ServicePresenter*, ContentPanel*> contents_;
  QMap<ServicePresenter*, QToolButton*> contacts_buttons_;
  GroupedBy<ServicePresenter*>* grouped_by_account_;
  MainButton* main_button_;
  ActivitiesButton* activities_button_;
  ActivitiesPanel* activities_;
  ContactsButton* contacts_button_;
  ContactsPanel* contacts_panel_;
};

} // namespace sdc

#endif // MAIN_WINDOW_H_
