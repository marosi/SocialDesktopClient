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

#include "abstract_model.h"
#include "content.h"
#include "Swiften/JID/JID.h"

class BcModel;
class ChannelController;

class BcContact : public sdc::Contact, public AbstractModel {
 public:
  BcContact(BcModel* model, const Swift::JID &jid);

  ChannelController* GetChannel();
  const Swift::JID& GetJID() { return jid_; }

  boost::signals2::signal<void ()> onChannelRetrieved;

 private:
  BcModel* model_;
  Swift::JID jid_;
};

#endif /* BC_CONTACT_H_ */
