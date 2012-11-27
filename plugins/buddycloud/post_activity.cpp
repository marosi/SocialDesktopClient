/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "post_activity.h"
#include "bc_presenter.h"
#include "channel.h"
#include "post.h"

PostActivity::PostActivity(BcPresenter* parent, const Post* post)
    : presenter_(parent), post_(post) {
  setObjectName("PostActivity");
  image_label()->setPixmap(presenter_->GetAvatar(post_->GetAuthorJID())->GetPixmap());
  QString text = QString::fromStdString(post_->GetAuthor() + " posted on " + post_->channel()->JID().toString());
  text_label()->setText(text);
}

void PostActivity::mouseDoubleClickEvent(QMouseEvent*) {
  presenter_->ShowChannel(post_->channel()->JID());
}
