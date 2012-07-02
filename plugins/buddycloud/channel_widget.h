/**
 * @file channel_widget.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 23, 2012
 */

#ifndef CHANNEL_WIDGET_H_
#define CHANNEL_WIDGET_H_

#include "sdc.h"
#include "buddycloud_bot.h"
#include "content/channel.h"
#include "content/post.h"
#include "ui_new_post_widget.h"
#include "boost/bind.hpp"
#include "boost/foreach.hpp"

class ChannelWidget : public sdc::ContentPanel {

    Q_OBJECT

 public:
  ChannelWidget(QWidget* parent, Channel::Ref channel)
      : sdc::ContentPanel(parent, channel), channel_(channel) {

    SetTitle(QString::fromStdString(channel->GetTitle()));

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
    AddHeaderButton(new_post_button_);

    new_post_ = new QWidget(this);
    new_post_ui.setupUi(new_post_);
    PutWidgetAboveContent(new_post_);
    new_post_->hide();

    // show hide new post widget
    connect(new_post_button_, SIGNAL(toggled(bool)),
        new_post_, SLOT(setVisible(bool)));
    // send button
    connect(new_post_ui.sendToolButton, SIGNAL(clicked()),
        this, SLOT(SendPost()));

    BOOST_FOREACH (Post1::Ref post, channel->GetPosts()) {
      AddPost(post);
    }

    qRegisterMetaType<Post1::Ref>("Post1::Ref");
    channel_->onPostAdded.connect(boost::bind(&ChannelWidget::OnPostAdded, this, _1));
    connect(this, SIGNAL(addPost(Post1::Ref)),
        this, SLOT(AddPost(Post1::Ref)));


    // TODO: Put this close action in more common content widget class
    channel_->onRemove.connect(boost::bind(&ChannelWidget::OnRemoveContent, this));
    connect(this, SIGNAL(_close()),
        this, SLOT(close()));
  }

 public slots:
  void SendPost() {
    Post1::Ref post(new Post1);
    post->SetContent(new_post_ui.textEdit->toPlainText().toStdString());

    channel_->controller()->Send(post);

    new_post_ui.textEdit->clear();
    new_post_->hide();
    new_post_button_->setChecked(false);
  }

  void AddPost(Post1::Ref post) {
    this->AddContent(post);
  }

 signals:
  void addPost(Post1::Ref post);
  void _close();

 private:
  void OnPostAdded(Post1::Ref post) {
    emit addPost(post);
  }
  void OnRemoveContent() {
    emit _close();
  }

  Channel::Ref channel_;
  QToolButton* new_post_button_;
  QWidget* new_post_;
  Ui::NewPostClass new_post_ui;
};

#endif /* CHANNEL_WIDGET_H_ */
