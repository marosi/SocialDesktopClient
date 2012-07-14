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
#include "Swiften/JID/JID.h"

class BcModel;
class ChannelController;

class BcContact : public sdc::Contact {
 public:
  BcContact(BcModel* model, const Swift::JID &jid);

  ChannelController* GetChannel();

  const std::string GetAvatarPath();

  boost::signals2::signal<void ()> onChannelRetrieved;
  boost::signals2::signal<void ()> onAvatarChanged;

 private:
  BcModel* model_;
  Swift::JID jid_;
};

#endif /* BC_CONTACT_H_ */
