#include "comment_widget.h"
#include "bc_presenter.h"
#include "ui_comment_widget.h"
#include "post.h"

CommentWidget::CommentWidget(AbstractPresenter* presenter, Comment* comment)
  : AbstractPresenter(presenter),
    ui(new Ui::CommentWidget),
    comment_(comment) {
  ui->setupUi(this);
  // set comment data
  ui->userLabel->setText(QString::fromStdString(comment->GetAuthor()));
  ui->contentTextBrowser->setText(QString::fromStdString(comment->GetContent()));
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(comment->GetAuthor());
  ui->avatarLabel->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), ui->avatarLabel, SLOT(setPixmap(QPixmap)));
}

CommentWidget::~CommentWidget() {
  delete ui;
}
