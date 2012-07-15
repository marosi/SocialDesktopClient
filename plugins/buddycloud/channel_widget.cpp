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
      PostWidget* pw = new PostWidget(this, post);
      posts_[post->GetID()] = pw;
      content_layout()->insertWidget(0, pw);
    }
  });
  sdc::bind(channel_->onChannelTitleChange, [&] (const std::string title) {
    title_label()->setText(QString::fromStdString(title));
  });
  sdc::bind(channel_->onChannelDescriptionChange, [&] (const std::string description) {
    setToolTip(QString::fromStdString(description));
  });
  sdc::bind(channel_->onPostAdded, [&] (Post1* post) {
    PostWidget* pw = new PostWidget(this, post);
    posts_[post->GetID()] = pw;
    content_layout()->insertWidget(0, pw);
  });
  sdc::bind(channel_->onPostDeleted, [&] (const std::string id) {
    PostWidget* pw = posts_[id];
    posts_.remove(id);
    delete pw;
  });
  // sync channel
  channel_->Sync();
}

void ChannelWidget::SendPost() {
  //Post1::Ref post(new Post1);
  //post->SetContent(new_post_ui.textEdit->toPlainText().toStdString());

  //channel_->controller()->CreatePost(post);

  Post1* post = new Post1(channel_);
  post->SetContent(new_post_ui.textEdit->toPlainText().toStdString());
  channel_->CreatePost(post); // TODO: rewrite CreatePost and CreatePost to recieve raw data instead of creating post, comment object

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
