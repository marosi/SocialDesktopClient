#include "content_panel.h"
#include "mainwindow.h"
#include "boost/cast.hpp"

namespace sdc {

ContentPanel::ContentPanel(QWidget *parent, Content::Ref content)
    : QFrame(parent), QtView(parent), widget_above_content_(0) {
	ui.setupUi(this);

	LOG(DEBUG) << "Content Panel Widget LIVES!!!!!";

	// close action
	connect(ui.closeButton, SIGNAL(clicked()),
	    this, SLOT(hide()));

	main_window_ = boost::polymorphic_downcast<MainWindow*>(parent);
	main_window_->AddContentPanel(this);
}

ContentPanel::~ContentPanel() {}

void ContentPanel::SetTitle(const QString &title) {
  ui.titleLable->setText(title);
}

void ContentPanel::AddContent(Content::Ref content) {
  QWidget* w = qtgui()->CreateContentWidget(content, this);
  QVBoxLayout* lay = boost::polymorphic_downcast<QVBoxLayout*>(ui.contentScrollArea->layout());
  lay->insertWidget(0, w);
}

void ContentPanel::AddHeaderButton(QAbstractButton* button) {
  ui.headerLayout->insertWidget(0, button);
}

void ContentPanel::PutWidgetAboveContent(QWidget* widget) {
  if (widget_above_content_) {
    delete widget_above_content_;
  }
  boost::polymorphic_downcast<QVBoxLayout*>(layout())->insertWidget(1, widget);
  widget_above_content_ = widget;
}

} /* namespace sdc */
