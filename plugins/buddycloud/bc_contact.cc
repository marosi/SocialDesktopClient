/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "bc_contact.h"
#include "bc_model.h"
#include "Swiften/Swiften.h"
#include "boost/filesystem.hpp"

BcContact::BcContact(BcModel* model, const Swift::JID &jid)
    : AbstractModel(model),
      model_(model),
      jid_(jid) {}

Channel* BcContact::GetChannel() {
  return model_->GetChannel(jid_);
}

void BcContact::Unfollow() {
  model()->UnsubscribeFromChannel(jid_);
}
