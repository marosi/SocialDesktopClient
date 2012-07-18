#include "contact_widget.h"
#include "main_window.h"
#include "boost/cast.hpp"
#include "boost/bind.hpp"

using boost::bind;

namespace sdc {

ContactWidget::ContactWidget(Contact* contact) {
	ui.setupUi(this);
	model_ = contact;

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
