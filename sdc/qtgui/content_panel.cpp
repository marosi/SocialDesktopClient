#include "content_panel.h"
#include "main_window.h"
#include <QMenu>
#include <QPainter>
#include <QStandardItemModel>
#include "boost/cast.hpp"


namespace sdc {

ContentPanel::ContentPanel(QWidget* parent)
    : QFrame(parent) {
  ui.setupUi(this);
  // close action
  QMenu* menu = new QMenu;
  QAction* close = menu->addAction("Close");
  ui.actionButton->setMenu(menu);
  connect(close, SIGNAL(triggered()),
      this, SLOT(close()));
}

ContentPanel::~ContentPanel() {}

void ContentPanel::AddTitleBarButton(QAbstractButton* button) {
  ui.headerLayout->insertWidget(0, button);
}

void ContentPanel::SetHeaderWidget(QWidget* widget) {
  if (ui.panelWidget) {
    delete ui.panelWidget;
  }
  ui.panelWidget = widget;
  ui.panelWidget->show();
  boost::polymorphic_downcast<QVBoxLayout*>(layout())->insertWidget(1, ui.panelWidget);
}

void ContentPanel::closeEvent(QCloseEvent *) {
  emit closed();
}

} /* namespace sdc */
