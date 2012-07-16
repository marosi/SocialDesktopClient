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
#include <QFile>
#include <QIcon>
#include <QPixmap>

#include "log.h"

using Swift::JID;

BcPresenter::BcPresenter() : AbstractPresenter(this) {}

void BcPresenter::Init() {
  LOG(DEBUG) << " BcPresenter thread ID: " << boost::this_thread::get_id();
  model_ = model<BcModel>();

  SetOwnAvatar(model_->GetOwnAvatarPath());
  UpdateAvatar(model_->GetOwnJID());

  QObject::connect(account_button()->menu()->addAction("Cuuuuz"), SIGNAL(triggered()), this, SLOT(Test()));

  sdc::bind(model_->onConnected, [&] () {
    channel_ = new ChannelWidget(this, model_->GetOwnChannel());
    main_window()->AddContentPanel(channel_);
    channel_->show();
  });

  sdc::bind(model_->onOwnAvatarChanged, [&] () {
    SetOwnAvatar(model_->GetOwnAvatarPath());
  });

  sdc::bind(model_->onAvatarChanged, [&] (const JID jid) {
    UpdateAvatar(jid);
  });

  sdc::bind(model_->onContactAdded, [&] (const JID jid) {
    BcContact* contact = model_->GetContact(jid);
    BcContactWidget* widget = new BcContactWidget(this, contact);
    main_window()->AddContact(this, widget);
  });
}

Avatar* BcPresenter::GetAvatar(const Swift::JID &jid) {
  if (avatars_.count(jid) > 0) {
    return avatars_[jid];
  } else {
    avatars_[jid] = new Avatar(model_->GetDefaultAvatarPath());
    return avatars_[jid];
  }
}

void BcPresenter::UpdateAvatar(const Swift::JID &jid) {
  if (avatars_.count(jid) > 0) {
    avatars_[jid]->Load(model_->GetAvatarPath(jid));
  } else {
    avatars_[jid] = new Avatar(model_->GetAvatarPath(jid));
  }
}

void BcPresenter::SetOwnAvatar(const std::string &file_path) {
  QString path = QString::fromStdString(file_path);
  if (QFile::exists(path))
    account_button()->tool_button()->setIcon(QIcon(path));
}
