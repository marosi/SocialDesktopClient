/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef AVATAR_FILESYSTEM_STORAGE_H_
#define AVATAR_FILESYSTEM_STORAGE_H_

#include "Swiften/Avatars/AvatarStorage.h"

namespace sdc {

class Account;

}

class AvatarFilesystemStorage : public Swift::AvatarStorage {
 public:
  AvatarFilesystemStorage(const std::string &service_dir, sdc::Account* account);
  virtual ~AvatarFilesystemStorage();

  virtual bool hasAvatar(const std::string& hash) const;
  virtual void addAvatar(const std::string& hash, const Swift::ByteArray& avatar);
  virtual Swift::ByteArray getAvatar(const std::string& hash) const;
  virtual boost::filesystem::path getAvatarPath(const std::string& hash) const;

  virtual void setAvatarForJID(const Swift::JID& jid, const std::string& hash);
  virtual std::string getAvatarForJID(const Swift::JID& jid) const;

 private:
  boost::filesystem::path defaultAvatarPath() const;
  boost::filesystem::path constructAvatarPath(const std::string &hash) const;

  boost::filesystem::path service_dir_;
  boost::filesystem::path account_dir_;
  sdc::Account* account_;
};

#endif // AVATAR_FILESYSTEM_STORAGE_H_
