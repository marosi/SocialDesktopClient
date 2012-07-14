/*
 * bc_contact.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 7, 2012
 */

#include "bc_contact.h"
#include "bc_model.h"
#include "Swiften/Swiften.h"
#include "boost/filesystem.hpp"

BcContact::BcContact(BcModel* model, const Swift::JID &jid)
    : model_(model),
      jid_(jid) {}

ChannelController* BcContact::GetChannel() {
  return model_->GetChannel(jid_);
}

const std::string BcContact::GetAvatarPath() {
  return model_->account()->GetProperty("avatars/" + jid_.toString());
}
