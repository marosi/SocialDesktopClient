#include "comment_widget.h"
#include "bc_presenter.h"
#include "post.h"
#include "ui_comment_widget.h"
#include "boost/date_time/posix_time/time_formatters.hpp"

CommentWidget::CommentWidget(AbstractPresenter* presenter, Comment* comment)
  : AbstractPresenter(presenter),
    ui(new Ui::CommentWidget),
    comment_(comment) {
  ui->setupUi(this);
  // set comment data
  ui->userLabel->setText(QString::fromStdString(comment->GetAuthor() + " " + boost::posix_time::to_simple_string(comment->GetPublished())));
  ui->contentTextBrowser->setText(QString::fromStdString(comment->GetContent()));
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(comment->GetAuthor());
  ui->avatarLabel->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), ui->avatarLabel, SLOT(setPixmap(QPixmap)));
}

CommentWidget::~CommentWidget() {
  delete ui;
}
