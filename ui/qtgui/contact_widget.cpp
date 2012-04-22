#include "contact_widget.h"
#include "mainwindow.h"
#include "boost/cast.hpp"
#include "boost/bind.hpp"

using boost::bind;

namespace sdc {

ContactWidget::ContactWidget(QWidget* parent, Contact::Ref contact)
    : QWidget(parent), QtView(parent) {
	ui.setupUi(this);
	model_ = contact;
	contact->onRemove.connect(bind(&ContactWidget::OnRemove, this));
	connect(this, SIGNAL(remove()),
	    this, SLOT(Remove()));

	MainWindow* parent_ = boost::polymorphic_downcast<MainWindow*>(parent);
	parent_->AddContact(this);

	ui.contactInfo->setText(QString::fromStdString(contact->GetUid()));
}

ContactWidget::~ContactWidget() {}

} /* namespace sdc */
