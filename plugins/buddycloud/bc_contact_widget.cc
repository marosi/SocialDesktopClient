#include "bc_contact_widget.h"
#include "bc_contact.h"
#include "bc_presenter.h"
#include "channel_widget.h"

#include "bind.h"
#include <QMouseEvent>

BcContactWidget::BcContactWidget(BcPresenter* presenter, BcContact* contact)
    : sdc::ContactWidget(contact),
      AbstractPresenter(presenter),
      contact_(contact),
      channel_(0) {
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(contact_->GetUid());
  avatar_label()->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), avatar_label(), SLOT(setPixmap(QPixmap)));
}

BcContactWidget::~BcContactWidget() {}

void BcContactWidget::mouseDoubleClickEvent(QMouseEvent* event) {
  event->ignore();
  presenter()->ShowChannel(contact_->GetJID());
}
