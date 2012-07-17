/*
 * channel_widget.cpp
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 10, 2012
 */

#include "channel_widget.h"
#include "post_widget.h"
#include "post.h"
#include "bind.h"
#include <QScrollBar>
#include <QtAlgorithms>
#include <algorithm>

ChannelWidget::ChannelWidget(AbstractPresenter* presenter, ChannelController* channel)
    : AbstractPresenter(presenter),
      channel_(channel) {
  { // set new post button
    new_post_button_ = new QToolButton;
    new_post_button_->setChecked(false);
    new_post_button_->setCheckable(true);
    new_post_button_->setToolTip("Post new message");
    new_post_button_->setText("+");
    QPalette p;
    QColor c;
    c.setNamedColor("yellowgreen");
    p.setColor(QPalette::Button, c);
    new_post_button_->setPalette(p);
    AddTitleBarButton(new_post_button_);
    new_post_ = new QWidget(this);
    new_post_ui.setupUi(new_post_);
    SetHeaderWidget(new_post_);
    new_post_->hide();
  }
  // show hide new post widget
  connect(new_post_button_, SIGNAL(toggled(bool)),
      new_post_, SLOT(setVisible(bool)));
  // send button
  connect(new_post_ui.sendToolButton, SIGNAL(clicked()),
      this, SLOT(SendPost()));
  // handle scrollbar event and retrieve new posts
  scroll_bar_ = content_scroll_area()->verticalScrollBar();
  connect(scroll_bar_, SIGNAL(valueChanged(int)),
      this, SLOT(OnScrollBarValueChanged(int)));
  // bind model events
  sdc::bind(channel_->onNewPostsRetrieved, [&] (const std::vector<Post1*> posts) {
    for (Post1* post : posts) {
      ShowPostInOrder(post);
    }
  });
  sdc::bind(channel_->onChannelTitleChange, [&] (const std::string title) {
    title_label()->setText(QString::fromStdString(title));
  });
  sdc::bind(channel_->onChannelDescriptionChange, [&] (const std::string description) {
    setToolTip(QString::fromStdString(description));
  });
  sdc::bind(channel_->onPostAdded, [&] (Post1* post) {
    ShowPostInOrder(post);
  });
  sdc::bind(channel_->onPostDeleted, [&] (const std::string id) {
    QList<Post1*>::iterator it = std::find_if(posts_order_.begin(), posts_order_.end(),
        [&] (const Post1* p) { return p->GetID() == id; });
    PostWidget* pw = posts_[*it];
    posts_.remove(*it);
    posts_order_.erase(it);
    delete pw;
  });
  // sync channel
  channel_->Sync();
}

void ChannelWidget::SendPost() {
  // publish
  channel_->PublishPost(new_post_ui.textEdit->toPlainText().toStdString());
  // tidy up text edit
  new_post_ui.textEdit->clear();
  new_post_->hide();
  new_post_button_->setChecked(false);
}

void ChannelWidget::OnScrollBarValueChanged(int value) {
  if (value == old_scroll_bar_value_)
    return;
  LOG(DEBUG) << "scroll bar max " << scroll_bar_->maximum() << " ... value : " << value;
  if (value >= scroll_bar_->maximum())
    channel_->RetrieveNextPosts();
  old_scroll_bar_value_ = value;
}

void ChannelWidget::ShowPostInOrder(Post1* post) {
  PostWidget* pw = new PostWidget(this, post);
  QList<Post1*>::iterator it = qUpperBound(posts_order_.begin(), posts_order_.end(), post,
      [&] (const Post1* p1, const Post1* p2) { return p1->GetPublished() > p2->GetPublished(); });
  posts_order_.insert(it, post);
  posts_[post] = pw;
  content_layout()->insertWidget(posts_order_.indexOf(post), pw);
}
