/**
 * @file bc_presenter.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jul 5, 2012
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

 private:
  void UpdateAvatar(const JID &jid);

  BcModel* model_;
  ChannelPanel* channel_;
  QMap<JID, ChannelPanel*> channels_;
  QList<BcContactWidget*> contacts_;
  QMap<JID, Avatar*> avatars_;
};

#endif /* BC_PRESENTER_H_ */
