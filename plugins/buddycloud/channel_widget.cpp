/*
 * channel_widget.cpp
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 10, 2012
 */

#include "channel_widget.h"
#include "post_frame.h"
#include "post.h"
#include "sdc/qtgui/bind.h"
#include <QScrollBar>
#include <QtAlgorithms>
#include "boost/foreach.hpp"
#include <algorithm>

ChannelPanel::ChannelPanel(AbstractPresenter* presenter, Channel* channel)
    : AbstractPresenter(presenter),
      channel_(channel) {
  this->hide();
  setObjectName("ChannelPanel");
  setMinimumWidth(300);

  // set new post button
  new_post_button_ = new QToolButton(this);
  new_post_button_->setObjectName("NewPostButton");
  new_post_button_->setCheckable(true);
  new_post_button_->setToolTip("New Post");
  AddTitleBarButton(new_post_button_);
  new_post_ = new QWidget(this);
  new_post_ui.setupUi(new_post_);
  SetHeaderWidget(new_post_);
  new_post_->hide();

  QToolButton* retrieve_next_btn_ = new QToolButton;
  retrieve_next_btn_->setText("Next");
  retrieve_next_btn_->setObjectName("NextButton");
  dynamic_cast<QVBoxLayout*>(content_scroll_area()->widget()->layout())->insertWidget(2, retrieve_next_btn_, Qt::AlignCenter);

  connect(retrieve_next_btn_, SIGNAL(clicked()), this, SLOT(RetrieveNext()));

  // show hide new post widget
  connect(new_post_button_, SIGNAL(toggled(bool)),
      new_post_, SLOT(setVisible(bool)));
  // send button
  connect(new_post_ui.postToolButton, SIGNAL(clicked()),
      this, SLOT(SendPost()));
  // handle scrollbar event and retrieve new posts
  scroll_bar_ = content_scroll_area()->verticalScrollBar();
  connect(scroll_bar_, SIGNAL(valueChanged(int)),
      this, SLOT(OnScrollBarValueChanged(int)));
  // bind model events
  sdc::bind(channel_->onNewPostsRetrieved, [&] (const std::vector<Post*> posts) {
    BOOST_FOREACH (Post* post , posts) {
      ShowPostInOrder(post);
    }
  });
  sdc::bind(channel_->onChannelTitleChange, [&] (const std::string title) {
    this->title_label()->setText(QString::fromStdString(title));
  });
  sdc::bind(channel_->onChannelDescriptionChange, [&] (const std::string description) {
    this->setToolTip(QString::fromStdString(description));
  });
  sdc::bind(channel_->onPostAdded, [&] (Post* post) {
    ShowPostInOrder(post);
  });
  sdc::bind(channel_->onPostDeleted, [&] (const std::string id) {
    QList<Post*>::iterator it = std::find_if(posts_order_.begin(), posts_order_.end(),
        [&] (const Post* p) { return p->GetID() == id; });
    PostFrame* pw = posts_[*it];
    posts_.remove(*it);
    posts_order_.erase(it);
    delete pw;
  });
  sdc::bind(channel_->onChannelAvailable, [&] () {
    this->show();
  });
  // sync channel
  channel_->Sync();
}

ChannelPanel::~ChannelPanel() {
  delete new_post_;
  delete new_post_button_;
  BOOST_FOREACH (PostFrame* widget , posts_.values())
    delete widget;
}

void ChannelPanel::SendPost() {
  // publish
  channel_->PublishPost(new_post_ui.textEdit->toPlainText().toStdString());
  // tidy up text edit
  new_post_ui.textEdit->clear();
  new_post_->hide();
  new_post_button_->setChecked(false);
}

void ChannelPanel::OnScrollBarValueChanged(int value) {
  if (value == old_scroll_bar_value_)
    return;
  if (value >= scroll_bar_->maximum())
    channel_->RetrieveNextPosts();
  old_scroll_bar_value_ = value;
}

void ChannelPanel::RetrieveNext() {
  channel_->RetrieveNextPosts();
}

void ChannelPanel::ShowPostInOrder(Post* post) {
  PostFrame* pw = new PostFrame(this, post);
  QList<Post*>::iterator it = qUpperBound(posts_order_.begin(), posts_order_.end(), post,
      [&] (const Post* p1, const Post* p2) { return p1->GetPublished() > p2->GetPublished(); });
  posts_order_.insert(it, post);
  posts_[post] = pw;
  content_layout()->insertWidget(posts_order_.indexOf(post), pw);
}
