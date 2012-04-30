#ifndef CONTENT_PANEL_H
#define CONTENT_PANEL_H

#include <QtGui/QFrame>
#include "ui_content_panel.h"
#include "content.h"
#include "qt_view.h"

namespace sdc {

class MainWindow;

class ContentPanel : public QFrame, public QtView {

    Q_OBJECT

 public:
  ContentPanel(QWidget *parent, Content::Ref content);
  virtual ~ContentPanel();

  void SetTitle(const QString &title);

  void AddContent(Content::Ref content);

  void AddHeaderButton(QAbstractButton* button);

  void PutWidgetAboveContent(QWidget* widget);

 protected:
  Ui::ContentPanelClass ui;
  MainWindow* main_window_;
  QWidget* widget_above_content_;
};

} /* namespace sdc */

#endif // CONTENT_PANEL_H
