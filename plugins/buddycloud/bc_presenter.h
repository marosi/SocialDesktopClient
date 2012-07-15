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

#include "service_presenter.h"
#include "avatar.h"
#include "Swiften/JID/JID.h"
#include <QMap>

class BcModel;

class BcPresenter : public sdc::ServicePresenter {

    Q_OBJECT

 public:
  void Init();
  Avatar* GetAvatar(const Swift::JID &jid);

 public slots:
  void Test();

 private:
  void SetAvatar(const std::string &file_path);

  BcModel* model_;
  QMap<Swift::JID, Avatar*> avatars_;
};

#endif /* BC_PRESENTER_H_ */
