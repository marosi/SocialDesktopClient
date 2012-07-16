#ifndef FILESYSTEM_STORAGES_H
#define FILESYSTEM_STORAGES_H

#include "Swiften/Client/Storages.h"
#include "Swiften/Client/MemoryStorages.h"
#include <string>

namespace sdc {

class AccountData;

}

class FilesystemStorages : public Swift::Storages {
 public:
  FilesystemStorages(const std::string &service_dir, sdc::AccountData* account);
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