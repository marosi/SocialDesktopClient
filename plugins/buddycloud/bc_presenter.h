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
#include "service_presenter.h"
#include "avatar.h"
#include "Swiften/JID/JID.h"
#include <QList>
#include <QMap>


class BcModel;
class BcContactWidget;
class ChannelWidget;

class BcPresenter : public sdc::ServicePresenter, public AbstractPresenter {

    Q_OBJECT

 public:
  BcPresenter();
  ~BcPresenter();
  void Init();

  Avatar* GetAvatar(const Swift::JID &jid);

 public slots:

 private:
  void SetOwnAvatar(const std::string &file_path);
  void UpdateAvatar(const Swift::JID &jid);

  BcModel* model_;
  ChannelWidget* channel_;
  QList<BcContactWidget*> contacts_;
  QMap<Swift::JID, Avatar*> avatars_;
};

#endif /* BC_PRESENTER_H_ */
