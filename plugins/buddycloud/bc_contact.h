/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BC_CONTACT_H_
#define BC_CONTACT_H_

#include "abstract_model.h"
#include "sdc/core/contact.h"
#include "Swiften/JID/JID.h"

class BcModel;
class Channel;

/**
 * buddycloud contact.
 */
class BcContact : public sdc::Contact, public AbstractModel {
 public:
  BcContact(BcModel* model, const Swift::JID &jid);

  /**
   * Returns contact's channel.
   * @return channel
   */
  Channel* GetChannel();

  /**
   * Gets contact's JID
   * @return JID
   */
  const Swift::JID& GetJID() { return jid_; }

  /**
   * Unfollows contact's channel.
   */
  void Unfollow();

  boost::signals2::signal<void ()> onChannelRetrieved;

 private:
  BcModel* model_;
  Swift::JID jid_;
};

#endif // BC_CONTACT_H_
