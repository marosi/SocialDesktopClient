/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "comment_activity.h"
#include "bc_presenter.h"
#include "channel.h"
#include "post.h"

CommentActivity::CommentActivity(BcPresenter* parent, const Swift::JID &from, const Swift::JID &to)
  : presenter_(parent), from_jid_(from), to_jid_(to) {
  setObjectName("CommentActivity");
  image_label()->setPixmap(presenter_->GetAvatar(from_jid_)->GetPixmap());
  QString text = QString::fromStdString(from_jid_.toString()+ " commented on " + to_jid_.toString());
  text_label()->setText(text);
}

void CommentActivity::mouseDoubleClickEvent(QMouseEvent *) {
  presenter_->ShowChannel(to_jid_);
}
