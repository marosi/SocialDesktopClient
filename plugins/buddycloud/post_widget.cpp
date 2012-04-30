#include "post_widget.h"
#include "boost/bind.hpp"
#include "buddycloud_bot.h"

PostWidget::PostWidget(QWidget *parent, Post1::Ref post)
    : QWidget(parent), sdc::QtView(parent), post_(post) {
	ui.setupUi(this);

	ui.authorLabel->setText(QString::fromStdString(post->GetAuthor()));
	ui.contentTextEdit->setText(QString::fromStdString(post->GetContent()));

	connect(ui.deleteButton, SIGNAL(clicked()),
	    this, SLOT(DeletePost()));

	post_->onRemove.connect(boost::bind(&PostWidget::OnRemove, this));
	connect(this, SIGNAL(remove()),
	    this, SLOT(close()));
}

PostWidget::~PostWidget() {}

void PostWidget::DeletePost() {
  QMessageBox confirm;
  confirm.setText("Do you really want to delete this post?");
  confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  confirm.setDefaultButton(QMessageBox::No);
  int result = confirm.exec();
  if (result == QMessageBox::Yes) {
    post_->controller()->Delete(post_);
  }
}
