#ifndef AVATAR_FILESYSTEM_STORAGE_H
#define AVATAR_FILESYSTEM_STORAGE_H

#include "Swiften/Avatars/AvatarStorage.h"

namespace sdc {

class AccountData;

}

class AvatarFilesystemStorage : public Swift::AvatarStorage {
 public:
  AvatarFilesystemStorage(const std::string &service_dir, sdc::AccountData* account);
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
  sdc::AccountData* account_;
};

#endif // AVATAR_FILESYSTEM_STORAGE_H
