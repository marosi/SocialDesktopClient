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
#include "qt_gui.h"
#include "service_presenter.h"
#include "settings_dialog.h"
#include <QToolButton>
#include <QGroupBox>
#include <QTreeWidget>
#include "boost/function.hpp"
#include "boost/cast.hpp"

namespace sdc {

template<class T>
class MainWindow::GroupedBy {
 public:
  GroupedBy(boost::function<QString (const ContactWidget*, const T)> titler)  {
    titler_ = titler;
    layout_ = new QVBoxLayout;
    tree_ = new QTreeWidget;
    tree_->setObjectName("ContactsTreeWidget");
    tree_->setStyleSheet(
      "QTreeWidget#ContactsTreeWidget {"
        "margin: 3px;"
      //"background-color: yellow;"
      //"background-clip: border;"
      //"background-origin: border;"
      "}"
      "QTreeWidget#ContactsTreeWidget::branch {"
        "border: none;"
      "}"
      "QTreeWidget#ContactsTreeWidget::branch:has-children:!has-siblings:closed {"
      "background: yellow;}"
      "QTreeWidget#ContactsTreeWidget::branch:has-children:open {"
      "background: red;}"
      "QTreeWidget#ContactsTreeWidget::item {"
        "padding: 3px;"
      "}"
    );
    tree_->setFrameStyle(QFrame::NoFrame); // a frame must be removed for the stylesheet to work
    tree_->viewport()->setObjectName("ContactsViewport");
    tree_->viewport()->setStyleSheet(
      "QWidget#ContactsViewport {"
      "border: 0px solid black;"
      "border-radius: 5px;"
      "background: white;"
      "}"
    );
    tree_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    tree_->setColumnCount(1);
    tree_->setHeaderHidden(true);
    //tree_->setRootIsDecorated(false);
    tree_->setIndentation(0);
    tree_->setAnimated(true);
    layout_->addWidget(tree_);
  }

  QLayout* GetLayout() { return layout_; }

  void Insert(ContactWidget* widget, const T key){
    if (!map_.contains(key)) {
      QTreeWidgetItem* item = new QTreeWidgetItem;
      item->setData(0, Qt::DisplayRole, titler_(widget, key));
      item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
      items_[key] = item;
      tree_->addTopLevelItem(item);
    }
    map_.insert(key, widget);
    QTreeWidgetItem* item = new QTreeWidgetItem;
    items_[key]->addChild(item);
    items_[key]->setExpanded(true);
    tree_->setItemWidget(item, 0, widget);
  }

  void RemoveGroup(const T key) {
    if (map_.contains(key)) {
      QTreeWidgetItem* item = items_[key];
      int i = tree_->indexOfTopLevelItem(item);
      tree_->takeTopLevelItem(i);
      items_.remove(key);
      map_.remove(key);
      delete item;
    }
  }

 private:
  boost::function<QString (const ContactWidget*, const T)> titler_;
  QMultiMap<T, ContactWidget*> map_;
  QVBoxLayout* layout_;
  QTreeWidget* tree_;
  QMap<T, QTreeWidgetItem*> items_;
};

MainWindow::MainWindow(QtGui* qtgui)
  : qtgui_(qtgui) {
  // setting Qt GUI layer object to the main View object
  ui.setupUi(this);
  ui.statusbar->hide();
  // grouping
  grouped_by_account_ = new GroupedBy<ServicePresenter*>(
      [&] (const ContactWidget*, const ServicePresenter* p) { return QString::fromStdString(p->model()->account()->GetUid()); });

  //layout()->setSizeConstraint(QLayout::SetFixedSize);

  // setup buttons
  main_button_ = new MainButton(this);
  ui.primesLayout->insertWidget(0, main_button_);

  contacts_button_ = new ContactsButton;
  ui.primesLayout->insertWidget(2, contacts_button_);

  activities_button_ = new ActivitiesButton;
  ui.primesLayout->insertWidget(3, activities_button_);

  // contacts
  contacts_panel_ = new ContactsPanel;
  contacts_button_->SetPanel(contacts_panel_);
  ui.contentFrame->layout()->addWidget(contacts_panel_);

  // set tree widget to whole scroll area of content panel not content pane,
  // because content pane has a spacer within that makes tree widget cover only a half of the height
  delete contacts_panel_->content_scroll_area()->layout();
  contacts_panel_->content_scroll_area()->setLayout(grouped_by_account_->GetLayout());

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
  std::string tooltip = qtgui_->GetService(parent)->name() +
      " - " +
      qtgui_->GetModel(parent)->account()->GetUid();
  contact->setToolTip(QString::fromStdString(tooltip));
  contacts_.insert(parent, contact);
  //contacts_panel_->content_layout()
  grouped_by_account_->Insert(contact, parent);
}

void MainWindow::RemoveAllContacts(ServicePresenter* parent) {
  grouped_by_account_->RemoveGroup(parent);
  for (ContactWidget* widget : contacts_.values(parent))
    delete widget;
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

} /* namespace sdc */
