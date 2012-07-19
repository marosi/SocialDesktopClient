/*
 * main_window.cpp
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 5, 2012
 */

#include "main_window.h"
#include "account.h"
#include "activities_button.h"
#include "activities_panel.h"
#include "contact_widget.h"
#include "contacts_button.h"
#include "contacts_panel.h"
#include "content_panel.h"
#include "log.h"
#include "main_button.h"
#include "qt_service_model.h"
#include "service_presenter.h"
#include "settings_dialog.h"
#include <QToolButton>
#include <QGroupBox>
#include "boost/function.hpp"

namespace sdc {

template<class T>
class MainWindow::GroupedBy {
 public:
  GroupedBy(boost::function<QString (const ContactWidget*, const T)> titler)  {
    titler_ = titler;
    layout_ = new QVBoxLayout();
  }

  QLayout* GetLayout() { return layout_; }

  void Insert(ContactWidget* widget, const T key){
    if (!map_.contains(key)) {
      QGroupBox* box = new QGroupBox(titler_(widget, key));
      box->setLayout(new QVBoxLayout);
      boxes_[key] = box;
      layout_->addWidget(boxes_[key]);
    }
    map_.insert(key, widget);
    boxes_[key]->layout()->addWidget(widget);
  }

  void RemoveGroup(const T key) {
    map_.remove(key);
    delete boxes_[key];
    boxes_.remove(key);
  }

 private:
  boost::function<QString (const ContactWidget*, const T)> titler_;
  QMultiMap<T, ContactWidget*> map_;
  QMap<T, QGroupBox*> boxes_;
  QVBoxLayout* layout_;
};

MainWindow::MainWindow(QtGui* qtgui) :
    QtView(qtgui) {
  // setting Qt GUI layer object to the main View object
  ui.setupUi(this);
  ui.statusbar->hide();
  connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(ShowSettingsDialog()));
  // grouping
  grouped_by_account_ = new GroupedBy<ServicePresenter*>(
      [&] (const ContactWidget*, const ServicePresenter* p) { return QString::fromStdString(p->model()->account()->GetUid()); });

  //layout()->setSizeConstraint(QLayout::SetFixedSize);

  main_button_ = new MainButton;
  ui.primesLayout->insertWidget(0, main_button_);

  contacts_button_ = new ContactsButton;
  ui.primesLayout->insertWidget(2, contacts_button_);

  activities_button_ = new ActivitiesButton;
  ui.primesLayout->insertWidget(3, activities_button_);

  // contacts
  contacts_panel_ = new ContactsPanel;
  contacts_button_->SetPanel(contacts_panel_);
  ui.contentFrame->layout()->addWidget(contacts_panel_);

  delete contacts_panel_->content_pane()->layout();
  contacts_panel_->content_pane()->setLayout(grouped_by_account_->GetLayout());

  // activities
  activities_ = new ActivitiesPanel;
  activities_button_->SetPanel(activities_);
  ui.contentFrame->layout()->addWidget(activities_);
}

MainWindow::~MainWindow() {}

void MainWindow::AddAccountButton(AccountButton* button) {
  ui.primesLayout->addWidget(button);
  buttons_ << button;
}

void MainWindow::RemoveAccountButton(AccountButton* button) {
  if (buttons_.contains(button)) {
    buttons_.removeOne(button);
    ui.primesLayout->removeWidget(button);
    delete button;
  }
}

void MainWindow::AddContact(ServicePresenter* parent, ContactWidget* contact) {
  std::string tooltip = qtgui()->GetService(parent)->name() +
      " - " +
      qtgui()->GetModel(parent)->account()->GetUid();
  contact->setToolTip(QString::fromStdString(tooltip));
  contacts_.insert(parent, contact);
  //contacts_panel_->content_layout()
  grouped_by_account_->Insert(contact, parent);
}

void MainWindow::RemoveAllContacts(ServicePresenter* parent) {
  for (ContactWidget* widget : contacts_.values(parent))
    delete widget;
  grouped_by_account_->RemoveGroup(parent);
  contacts_.remove(parent);
}

void MainWindow::AddContentPanel(ServicePresenter* parent, ContentPanel* panel) {
  contents_.insert(parent, panel);
  ui.contentFrame->layout()->addWidget(panel);
}

void MainWindow::RemoveAllContentPanels(ServicePresenter* parent) {
  for (ContentPanel* panel : contents_.values(parent))
    delete panel;
  contents_.remove(parent);
}

void MainWindow::ShowSettingsDialog() {
  settings_ = new SettingsDialog(this);
}

} /* namespace sdc */
