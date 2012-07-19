#ifndef CONTENT_PANEL_H
#define CONTENT_PANEL_H

#include "ui_content_panel.h"
#include <QtGui/QFrame>
#include "sdc/core/contact.h"
#include "boost/cast.hpp"

namespace sdc {

class MainWindow;

class ContentPanel : public QFrame {

    Q_OBJECT

 public:
  ContentPanel(QWidget* parent = 0);
  virtual ~ContentPanel();

  void AddTitleBarButton(QAbstractButton* button);

  void SetHeaderWidget(QWidget* widget);

  QLabel* title_label() {
    return ui.titleLabel;
  }

  QScrollArea* content_scroll_area() {
    return ui.scrollArea;
  }

  QVBoxLayout* content_layout() {
    return boost::polymorphic_downcast<QVBoxLayout*>(ui.contentScrollArea->layout());
  }


 protected:
  Ui::ContentPanelClass ui;
  MainWindow* main_window_;
};

} /* namespace sdc */

#endif // CONTENT_PANEL_H
