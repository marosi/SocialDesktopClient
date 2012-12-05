/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "bc_contact_widget.h"
#include "bc_contact.h"
#include "bc_presenter.h"
#include "channel_widget.h"
#include "sdc/qtgui/bind.h"
#include <QMouseEvent>
#include <QMenu>

BcContactWidget::BcContactWidget(BcPresenter* presenter, BcContact* contact)
    : sdc::ContactWidget(contact),
      AbstractPresenter(presenter),
      contact_(contact) {
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(contact_->GetUid());
  avatar_label()->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), avatar_label(), SLOT(setPixmap(QPixmap)));
  // unfollow action
  unfollow_ = new QAction("Unfollow", this);
  connect(unfollow_, SIGNAL(triggered()), this, SLOT(Unfollow()));
}

BcContactWidget::~BcContactWidget() {}

void BcContactWidget::mouseDoubleClickEvent(QMouseEvent* event) {
  event->ignore();
  // show contact's channel
  presenter()->ShowChannel(contact_->GetJID());
}

void BcContactWidget::contextMenuEvent(QContextMenuEvent* event) {
   QMenu menu(this);
   menu.addAction(unfollow_);
   menu.exec(event->globalPos());
}

void BcContactWidget::Unfollow() {
  contact_->Unfollow();
}
