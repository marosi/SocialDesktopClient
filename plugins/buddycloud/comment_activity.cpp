#include "comment_activity.h"
#include "bc_presenter.h"
#include "channel.h"
#include "post.h"

CommentActivity::CommentActivity(BcPresenter* parent, const Comment* comment)
    : presenter_(parent), comment_(comment) {
  image_label()->setPixmap(presenter_->GetAvatar(comment_->GetAuthorJID())->GetPixmap());
  QString text = QString::fromStdString(comment_->GetAuthor() + " commented on " + comment_->post()->channel()->JID().toString());
  text_label()->setText(text);
}

void CommentActivity::mouseDoubleClickEvent(QMouseEvent *) {
  presenter_->ShowChannel(comment_->post()->channel()->JID());
}
