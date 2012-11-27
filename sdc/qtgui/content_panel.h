/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CONTENT_PANEL_H_
#define CONTENT_PANEL_H_

#include "ui_content_panel.h"
#include <QFrame>
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

  QWidget* content_pane() {
    return ui.contentWiget;
  }

  QVBoxLayout* content_layout() {
    return boost::polymorphic_downcast<QVBoxLayout*>(ui.contentWiget->layout());
  }

  QToolButton* action_button() {
    return ui.actionButton;
  }

 signals:
  void closed();

 protected:
  virtual void closeEvent(QCloseEvent *);

 private:
  Ui::ContentPanel ui;
  MainWindow* main_window_;
};

} // namespace sdc

#endif // CONTENT_PANEL_H_
