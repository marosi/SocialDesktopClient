#include "contact_widget.h"

namespace sdc {

ContactWidget::ContactWidget(QWidget *parent, Contact::Ref contact)
    : QWidget(parent), QtView(parent) {
	ui.setupUi(this);
	ui.contactInfo->setText(QString::fromStdString(contact->GetUid()));
}

ContactWidget::~ContactWidget() {}

} /* namespace sdc */
