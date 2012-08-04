/*
 * bc_presenter.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 7, 2012
 */

#include "bc_presenter.h"
#include "bc_contact_widget.h"
#include "bc_model.h"
#include "channel.h"
#include "channel_widget.h"
#include "comment_activity.h"
#include "post.h"
#include "post_activity.h"
#include "sdc/core/log.h"
#include "sdc/qtgui/activity.h"
#include "sdc/qtgui/activities_panel.h"
#include <QFile>
#include <QIcon>
#include <QLineEdit>
#include <QMenu>
#include <QPixmap>
#include <QWidgetAction>

#include "sdc/core/log.h"

BcPresenter::BcPresenter() : AbstractPresenter(this), channel_(0) {}

BcPresenter::~BcPresenter() {}

void BcPresenter::Init() {
  model_ = model<BcModel>();
  qRegisterMetaType<JID>("JID");

  // set avatar
  UpdateAvatar(model_->GetOwnJID());

  // set buddycloud icon on account button
  QIcon icon(":/icon.svg");
  account_button()->button()->setIcon(icon);
  account_button()->button()->setIconSize(QSize(32, 32));

  connect(account_button()->title_action(), SIGNAL(triggered()),
          this, SLOT(ShowOwnChannel()));

  // set subscribe to channel action
  QMenu* subscribe = account_button()->button()->menu()->addMenu("Show Channel");
  QWidgetAction* edit = new QWidgetAction(account_button()->button()->menu());
  subscribe_to_ = new QLineEdit;
  subscribe_to_->setMinimumWidth(200);
  edit->setDefaultWidget(subscribe_to_);
  subscribe->addAction(edit);
  connect(subscribe_to_, SIGNAL(returnPressed()), this, SLOT(OnShowChannelLineEditEnter()));


  // set stylesheet
  QFile file("plugins/buddycloud/resources/bc.qss");
  file.open(QFile::ReadOnly);
  QString stylesheet = QLatin1String(file.readAll());
  SetStylesheet(stylesheet);

  // bindings
  sdc::bind(model_->onConnected, [&] () {
    if (!channel_) {
      channel_ = new ChannelPanel(this, model_->GetOwnChannel());
      channels_[model_->GetOwnJID()] = channel_;
      this->main_window()->AddContentPanel(this, channel_);
    }
    channel_->show();
  });

  sdc::bind(model_->onOwnAvatarChanged, [&] () {
    //SetOwnAvatar(model_->GetOwnAvatarPath());
  });

  sdc::bind(model_->onAvatarChanged, [&] (const JID jid) {
    UpdateAvatar(jid);
  });

  sdc::bind(model_->onContactAdded, [&] (const JID jid) {
    BcContact* contact = model_->GetContact(jid);
    BcContactWidget* widget = new BcContactWidget(this, contact);
    contacts_.append(widget);
    this->main_window()->AddContact(this, widget);
  });

  sdc::bind(model_->onNewPost, [&] (const Post* post) {
    PostActivity* act = new PostActivity(this, post);
    this->main_window()->activities()->AddActivity(act);
  });

  sdc::bind(model_->onNewComment, [&] (const Comment* comment) {
    CommentActivity* act = new CommentActivity(this, comment);
    this->main_window()->activities()->AddActivity(act);
  });

  model_->Connect();
}

Avatar* BcPresenter::GetAvatar(const JID &jid) {
  if (avatars_.count(jid) > 0) {
    return avatars_[jid];
  } else {
    avatars_[jid] = new Avatar(model_->GetDefaultAvatarPath());
    return avatars_[jid];
  }
}

void BcPresenter::ShowChannel(const JID &jid) {
  if (channels_.contains(jid))
    channels_[jid]->show();
  else {
    ChannelPanel* cw = new ChannelPanel(this, model_->GetChannel(jid.toString()));
    channels_[jid] = cw;
    main_window()->AddContentPanel(this, cw);
    cw->show();
  }
}

void BcPresenter::ShowOwnChannel() {
  channel_->show();
}

void BcPresenter::OnShowChannelLineEditEnter() {
 ShowChannel(subscribe_to_->text().toStdString());
}

void BcPresenter::UpdateAvatar(const JID &jid) {
  if (avatars_.count(jid) > 0) {
    avatars_[jid]->Load(model_->GetAvatarPath(jid));
  } else {
    avatars_[jid] = new Avatar(model_->GetAvatarPath(jid));
  }
}
