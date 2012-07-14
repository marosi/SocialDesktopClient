/*
 * bc_presenter.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 7, 2012
 */

#include "bc_presenter.h"
#include "bc_contact_widget.h"
#include "Swiften/JID/JID.h"
#include <QFile>
#include <QIcon>
#include <QPixmap>

using Swift::JID;

void BcPresenter::Init() {
  LOG(DEBUG) << " BcPresenter thread ID: " << boost::this_thread::get_id();
  model_ = model<BcModel>();

  QObject::connect(account_button()->menu()->addAction("Cuuuuz"), SIGNAL(triggered()), this, SLOT(Test()));

  sdc::bind(model_->onConnected, [&] () {

  });

  sdc::bind(model_->onOwnAvatarChange, [&] (const std::string file_path) {
    QString path = QString::fromStdString(file_path);
    if (QFile::exists(path))
      account_button()->tool_button()->setIcon(QIcon(path));
  });

  sdc::bind(model_->onContactAdded, [&] (const JID jid) {
    BcContact* contact = model_->GetContact(jid);
    BcContactWidget* widget = new BcContactWidget(main_window(), contact);
    main_window()->AddContact(this, widget);
  });
}

  void BcPresenter::Test() {
    model_->test();
  }
