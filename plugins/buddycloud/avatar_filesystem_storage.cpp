#include "avatar_filesystem_storage.h"
#include "boost/filesystem.hpp"
#include "account_data.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Base/ByteArray.h"
#include <fstream>

using namespace boost::filesystem;
using namespace Swift;

AvatarFilesystemStorage::AvatarFilesystemStorage(const std::string &service_dir, sdc::AccountData* account)
    : service_dir_(service_dir), account_dir_(account->GetDir()), account_(account)  {LOG(DEBUG) << "IN AFS CTOR";}

AvatarFilesystemStorage::~AvatarFilesystemStorage() {}

bool AvatarFilesystemStorage::hasAvatar(const std::string& hash) const {
  /*
   * This should return always true
   * since there is default avatar file always present
   */
  path p = constructAvatarPath(hash);
  return exists(p);
}

void AvatarFilesystemStorage::addAvatar(const std::string& hash, const ByteArray& avatar) {
  path p = constructAvatarPath(hash);
  std::ofstream file;
  file.open(p.file_string());
  file << byteArrayToString(avatar);
  file.close();
}

ByteArray AvatarFilesystemStorage::getAvatar(const std::string& hash) const {
  ByteArray result;
  readByteArrayFromFile(result, getAvatarPath(hash).file_string());
  return result;
}

boost::filesystem::path AvatarFilesystemStorage::getAvatarPath(const std::string& hash) const {
  path result = constructAvatarPath(hash);
  if (exists(result))
    return result;
  else
    return defaultAvatarPath();
}

void AvatarFilesystemStorage::setAvatarForJID(const JID& jid, const std::string& hash) {
  account_->SetProperty("avatars/" + jid.toString(), hash);
}

std::string AvatarFilesystemStorage::getAvatarForJID(const JID& jid) const {
  return account_->GetProperty("avatars/" + jid.toString());
}

boost::filesystem::path AvatarFilesystemStorage::defaultAvatarPath() const {
  return service_dir_ / "default_avatar.png";
}

boost::filesystem::path AvatarFilesystemStorage::constructAvatarPath(const std::string &hash) const {
  return account_dir_ / hash;
}
