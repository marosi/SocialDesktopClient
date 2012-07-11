#include "post_widget.h"
#include "bc_model.h"
#include "comment_widget.h"
// from sdc
#include "bind.h"

PostWidget::PostWidget(Post1* post)
    : post_(post) {
	ui.setupUi(this);
  ui.commentLineEdit->hide();
  // set post data
	ui.authorLabel->setText(QString::fromStdString(post_->GetAuthor()));
	ui.contentTextEdit->setText(QString::fromStdString(post_->GetContent()));
  // set focusing policy
  setFocusPolicy(Qt::StrongFocus);
  setFocusProxy(ui.commentLineEdit);
  // bindings
	connect(ui.deleteButton, SIGNAL(clicked()),
	    this, SLOT(DeletePost()));
  connect(ui.commentLineEdit, SIGNAL(returnPressed()),
          this, SLOT(PostComment()));
  sdc::bind(post_->onCommentAdded, [&] (Comment* comment) {
            LOG(DEBUG) << "ADDING COMMENT : PostWidget 1";
    CommentWidget* cw = new CommentWidget(comment);
    comments_.append(cw);
    ui.commentsLayout->addWidget(cw);
  });
  // show comments
  for (Comment* comment : post_->comments()) {
    CommentWidget* cw = new CommentWidget(comment);
    comments_.append(cw);
    ui.commentsLayout->addWidget(cw);
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
