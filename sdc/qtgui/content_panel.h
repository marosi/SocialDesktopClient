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

/**
 * Common panel class for all panels within the content area of the main window.
 */
class ContentPanel : public QFrame {

    Q_OBJECT

 public:
  ContentPanel(QWidget* parent = 0);
  virtual ~ContentPanel();

  /**
   * Adds button on the left of panel title (the upper left corner).
   * @param button a button instance
   */
  void AddTitleBarButton(QAbstractButton* button);

  /**
   * Sets widget inside panel header, that is between the title and content scroll area.
   * @param widget a widget instance
   */
  void SetHeaderWidget(QWidget* widget);

  /**
   * Gets panel title.
   * @return title label
   */
  QLabel* title_label() {
    return ui.titleLabel;
  }

  /**
   * Gets top content scroll area.
   * @return scroll area
   */
  QScrollArea* content_scroll_area() {
    return ui.scrollArea;
  }

  /**
   * Content pane for most of the panel's content.
   * @return content widget
   */
  QWidget* content_pane() {
    return ui.contentWiget;
  }

  /**
   * Gets content pane layout.
   * @return content pane layout
   */
  QVBoxLayout* content_layout() {
    return boost::polymorphic_downcast<QVBoxLayout*>(ui.contentWiget->layout());
  }

  /**
   * Get panel's action button situated in the upper right corner of the panel.
   * @return action button
   */
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
