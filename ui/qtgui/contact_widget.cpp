#include "contact_widget.h"
#include "main_window.h"
#include "boost/cast.hpp"
#include "boost/bind.hpp"

using boost::bind;

namespace sdc {

ContactWidget::ContactWidget(QWidget* parent, Contact::Ref contact)
    : QWidget(parent), QtView(parent) {
	ui.setupUi(this);
	model_ = contact;


	// TODO: put closing widget on content signal in more common content widget class
  //contact->onRemove.connect(bind(&ContactWidget::Close, this));
	connect(this, SIGNAL(_close()),
	    this, SLOT(close()));

	MainWindow* parent_ = boost::polymorphic_downcast<MainWindow*>(parent);


	ui.contactInfo->setText(QString::fromStdString(contact->GetUid()));

	close_button_ = new QToolButton;
	close_button_->setText("x");
	close_button_->setMaximumHeight(20);
	close_button_->setMaximumWidth(20);
	boost::polymorphic_downcast<QBoxLayout*>(layout())->addWidget(close_button_, 0, Qt::AlignRight | Qt::AlignTop);

	connect(close_button_, SIGNAL(clicked()),
	    this, SLOT(Remove()));
}

ContactWidget::~ContactWidget() {}

} /* namespace sdc */
