/*
 * bc_contact.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 7, 2012
 */

#include "bc_contact.h"
#include "bc_model.h"
#include "Swiften/Swiften.h"

BcContact::BcContact(BcModel* model, const Swift::JID &jid)
    : model_(model),
      jid_(jid),
      client_(model->xmpp()),
      router_(client_->getIQRouter()) {

}

ChannelController* BcContact::GetChannel() {
  return model_->GetChannel(jid_);
}
