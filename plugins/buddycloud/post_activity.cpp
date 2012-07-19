#include "post_activity.h"
#include "bc_presenter.h"
#include "channel.h"
#include "post.h"

PostActivity::PostActivity(BcPresenter* parent, const Post* post)
    : presenter_(parent), post_(post) {
  image_label()->setPixmap(presenter_->GetAvatar(post_->GetAuthorJID())->GetPixmap());
  QString text = QString::fromStdString(post_->GetAuthor() + " posted on " + post_->channel()->JID().toString());
  text_label()->setText(text);
}

void PostActivity::mouseDoubleClickEvent(QMouseEvent*) {
  presenter_->ShowChannel(post_->channel()->JID());
}
