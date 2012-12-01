/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BC_PRESENTER_H_
#define BC_PRESENTER_H_

#include "abstract_presenter.h"
#include "avatar.h"
#include "sdc/qtgui/service_presenter.h"
#include "Swiften/JID/JID.h"
#include <QList>
#include <QMap>


class BcModel;
class BcContactWidget;
class ChannelPanel;
class Error;

class BcPresenter : public sdc::ServicePresenter, public AbstractPresenter {

    Q_OBJECT

 public:
  typedef Swift::JID JID;

  BcPresenter();
  ~BcPresenter();
  void Init();

  Avatar* GetAvatar(const JID &jid);

 public slots:
  void ShowChannel(const JID &jid);
  void ShowOwnChannel();
  void OnShowChannelLineEditEnter();
  void ShowSubscribers();
  void ShowFollowers();

 private:
  void UpdateAvatar(const JID &jid);

  BcModel* model_;
  ChannelPanel* channel_;
  QMap<JID, ChannelPanel*> channels_;
  QList<BcContactWidget*> contacts_;
  QMap<JID, Avatar*> avatars_;
  QLineEdit* subscribe_to_;
  QVBoxLayout* subscribers_layout_;
  int subscribers_pos_;
  QWidget* followers_;
  int followers_pos_;
};

#endif // BC_PRESENTER_H_
