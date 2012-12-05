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
#include <QAction>
#include <QMouseEvent>
#include <QMenu>

BcContactWidget::BcContactWidget(BcPresenter* presenter, BcContact* contact)
    : sdc::ContactWidget(contact),
      AbstractPresenter(presenter),
      contact_(contact),
      channel_(0) {
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(contact_->GetUid());
  avatar_label()->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), avatar_label(), SLOT(setPixmap(QPixmap)));
  // context menu
  setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(ShowContextMenu(QPoint)));

  follow_ = new QAction("Follow", this);
  unfollow_ = new QAction("Unfollow", this);
  menu_ = new QMenu;
  menu_->addAction(follow_);
  menu_->addAction(unfollow_);
}

BcContactWidget::~BcContactWidget() {}

void BcContactWidget::mouseDoubleClickEvent(QMouseEvent* event) {
  event->ignore();
  // show contact's channel
  presenter()->ShowChannel(contact_->GetJID());
}

void BcContactWidget::ShowContextMenu(QPoint position) {
  QPoint global = this->mapToGlobal(position);
  QAction* selectedItem = menu_->exec(global);
  //if (selectedItem)  {}
  //else  {}
}
