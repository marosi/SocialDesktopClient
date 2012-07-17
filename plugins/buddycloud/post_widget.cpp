#include "post_widget.h"
#include "bc_model.h"
#include "bc_presenter.h"
#include "comment_widget.h"
#include "post.h"
// from sdc
#include "bind.h"
#include "boost/date_time/posix_time/time_formatters.hpp"

PostWidget::PostWidget(AbstractPresenter* presenter, Post1* post)
    : AbstractPresenter(presenter),
      post_(post) {
	ui.setupUi(this);
  ui.commentLineEdit->hide();
  // set post data
  ui.authorLabel->setText(QString::fromStdString(post_->GetAuthor() + " " + boost::posix_time::to_simple_string(post_->GetPublished())));
  ui.contentTextEdit->setText(QString::fromStdString(post_->GetContent()));
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(post_->GetAuthor());
  ui.avatarLabel->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), ui.avatarLabel, SLOT(setPixmap(QPixmap)));
  // set focusing policy
  setFocusPolicy(Qt::StrongFocus);
  setFocusProxy(ui.commentLineEdit);
  // bindings
	connect(ui.deleteButton, SIGNAL(clicked()),
	    this, SLOT(DeletePost()));
  connect(ui.commentLineEdit, SIGNAL(returnPressed()),
          this, SLOT(PostComment()));
  sdc::bind(post_->onCommentAdded, [&] (Comment* comment) {
    ShowCommentInOrder(comment);
  });
  // show comments
  for (Comment* comment : post_->comments()) {
    ShowCommentInOrder(comment);
  }
}

PostWidget::~PostWidget() {
  for (CommentWidget* widget : comments_)
    delete widget;
}

void PostWidget::DeletePost() {
  QMessageBox confirm;
  confirm.setText("Do you really want to delete this post?");
  confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  confirm.setDefaultButton(QMessageBox::No);
  int result = confirm.exec();
  if (result == QMessageBox::Yes) {
    post_->Delete();
  }
}

void PostWidget::mouseReleaseEvent(QMouseEvent *) {
  ui.commentLineEdit->show();
  setFocus();
}

void PostWidget::PostComment() {
  post_->PostComment(ui.commentLineEdit->text().toStdString());
  ui.commentLineEdit->clear();
}

void PostWidget::ShowCommentInOrder(Comment* comment) {
  QList<Comment*>::iterator it = qUpperBound(comments_order_.begin(), comments_order_.end(), comment,
      [&] (const Comment* c1, const Comment* c2) { return c1->GetPublished() < c2->GetPublished(); });
  comments_order_.insert(it, comment);
  CommentWidget* cw = new CommentWidget(this, comment);
  comments_[comment] = cw;
  ui.commentsLayout->insertWidget(comments_order_.indexOf(comment), cw);
}
