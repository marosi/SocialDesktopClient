/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "filesystem_storages.h"
#include "avatar_filesystem_storage.h"
#include "Swiften/VCards/VCardMemoryStorage.h"
#include "Swiften/Disco/CapsMemoryStorage.h"
#include "Swiften/Roster/RosterMemoryStorage.h"

FilesystemStorages::FilesystemStorages(const std::string &service_dir, sdc::Account* account) {
  vcard_storage_ = new Swift::VCardMemoryStorage();
  avatar_storage_ = new AvatarFilesystemStorage(service_dir, account);
  caps_storage_ = new Swift::CapsMemoryStorage();
  roster_storage_ = new Swift::RosterMemoryStorage();
}

FilesystemStorages::~FilesystemStorages() {
  delete vcard_storage_;
  delete avatar_storage_;
  delete caps_storage_;
  delete roster_storage_;
}

Swift::VCardStorage* FilesystemStorages::getVCardStorage() const {
  return vcard_storage_;
}

Swift::AvatarStorage* FilesystemStorages::getAvatarStorage() const {
  return avatar_storage_;
}

Swift::CapsStorage* FilesystemStorages::getCapsStorage() const {
  return caps_storage_;
}

Swift::RosterStorage* FilesystemStorages::getRosterStorage() const {
  return roster_storage_;
}
