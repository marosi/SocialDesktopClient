/**
 * @file bc_contact.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 24, 2012
 */

#ifndef BC_CONTACT_H_
#define BC_CONTACT_H_

#include "content.h"
#include "Swiften/Client/Client.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Queries/IQRouter.h"

class BcModel;
class ChannelController;

class BcContact : public sdc::Contact {
 public:
  typedef boost::shared_ptr<BcContact> Ref;

  BcContact(BcModel* model, const Swift::JID &jid);

  ChannelController* GetChannel();

  boost::signals2::signal<void ()> onChannelRetrieved;

 private:
  BcModel* model_;
  Swift::JID jid_;
  Swift::Client* client_;
  Swift::IQRouter* router_;
};

#endif /* BC_CONTACT_H_ */
