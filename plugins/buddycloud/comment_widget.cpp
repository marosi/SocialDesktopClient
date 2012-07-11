#include "comment_widget.h"
#include "ui_comment_widget.h"
#include "post.h"

CommentWidget::CommentWidget(Comment* comment)
  : ui(new Ui::CommentWidget), comment_(comment) {
  ui->setupUi(this);
  // set comment data
  ui->userLabel->setText(QString::fromStdString(comment->GetAuthor()));
  ui->contentTextBrowser->setText(QString::fromStdString(comment->GetContent()));
}

CommentWidget::~CommentWidget() {
  delete ui;
}
