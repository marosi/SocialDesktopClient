/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef FILESYSTEM_STORAGES_H_
#define FILESYSTEM_STORAGES_H_

#include "Swiften/Client/Storages.h"
#include "Swiften/Client/MemoryStorages.h"
#include <string>

namespace sdc {

class Account;

}

/**
 * Filesystem storage for various XMPP client's data.
 */
class FilesystemStorages : public Swift::Storages {
 public:
  FilesystemStorages(const std::string &service_dir, sdc::Account* account);
  ~FilesystemStorages();

  Swift::VCardStorage* getVCardStorage() const;
  Swift::AvatarStorage* getAvatarStorage() const;
  Swift::CapsStorage* getCapsStorage() const;
  Swift::RosterStorage* getRosterStorage() const;

 private:
  Swift::VCardStorage* vcard_storage_;
  Swift::AvatarStorage* avatar_storage_;
  Swift::CapsStorage* caps_storage_;
  Swift::RosterStorage* roster_storage_;
};

#endif // FILESYSTEM_STORAGES_H
