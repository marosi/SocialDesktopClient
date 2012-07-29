#include "comment_frame.h"
#include "bc_presenter.h"
#include "post.h"
#include "ui_comment_frame.h"
#include "boost/date_time/posix_time/time_formatters.hpp"

CommentFrame::CommentFrame(AbstractPresenter* presenter, Comment* comment)
  : AbstractPresenter(presenter),
    ui(new Ui::CommentFrame),
    comment_(comment) {
  ui->setupUi(this);
  // set comment data
  ui->authorLabel->setText(QString::fromStdString(comment->GetAuthor()));
  ui->contentLabel->setText(QString::fromStdString(comment->GetContent()));
  setToolTip(QString::fromStdString(boost::posix_time::to_simple_string(comment->GetPublished())));
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(comment->GetAuthor());
  ui->avatarLabel->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), ui->avatarLabel, SLOT(setPixmap(QPixmap)));
}

CommentFrame::~CommentFrame() {
  delete ui;
}
