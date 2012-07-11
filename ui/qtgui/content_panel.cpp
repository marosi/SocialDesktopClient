#include "content_panel.h"
#include "main_window.h"
#include <QStandardItemModel>
#include "boost/cast.hpp"

namespace sdc {

ContentPanel::ContentPanel(QWidget* parent)
    : QFrame(parent) {
  ui.setupUi(this);
  // close action
  connect(ui.closeButton, SIGNAL(clicked()),
      this, SLOT(hide()));
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

} /* namespace sdc */
