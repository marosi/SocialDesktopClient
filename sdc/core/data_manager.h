/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_DATA_MANAGER_H_
#define CORE_DATA_MANAGER_H_

#include "account.h"
#include "abstract_manager.h"
#include "properties.h"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/signals2.hpp"
#include <string>
#include <vector>
#include <map>

namespace sdc {

/**
 * Structure for library persistent data.
 */
class Library {
 public:
  friend class boost::serialization::access;

 private:
  bool enabled_; /**< library enabled state */
  std::string filename_; /**< library actual file name */

  /**
   * Boost serialization method
   */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    if (version) {}
    ar & BOOST_SERIALIZATION_NVP(enabled_);
    ar & BOOST_SERIALIZATION_NVP(filename_);
  }
};

class Account;


/**
 * DataManager maintains SocialDesktopClient configuration,
 * while providing interface to work with application persistent data.
 */
class DataManager : public AbstractManager, public Properties {
 public:
  friend class boost::serialization::access;
  friend class Core;

  DataManager(Core* core) : AbstractManager(core) {}
  void Init();

  void SetEnabledAccount(int index, bool is_enabled) {
    if (accounts_[index]->IsEnabled() != is_enabled) {
      accounts_[index]->SetEnabled(is_enabled);
      onAccountsChanged();
      if (is_enabled) {
        onAccountEnabled(accounts_[index]);
      } else {
        onAccountDisabled(accounts_[index]);
      }
    }
  }

  /**
   * Adds account to the persistent storage.
   * @param account account data
   */
  void AddAccount(Account* account) {
    accounts_.push_back(account);
    SetEnabledAccount(accounts_.size() - 1, true);
    onAccountsChanged();
  }

  /**
   * Removes account from storage.
   * @param index account index
   */
  void RemoveAccount(int index) {
    onAccountDisabled(accounts_[index]);
    accounts_.erase(accounts_.begin() + index);
    onAccountsChanged();
  }

  /**
   * Gets account based on its index.
   * @param index account index
   * @return Account instance
   */
  Account* GetAccount(int index) {
    return accounts_[index];
  }

  /**
   * Gets all configured accounts.
   * @return vector of accounts
   */
  std::vector<Account*> GetAccounts() const {
    return accounts_;
  }

  /**
   * Exit actions like data serialization.
   */
  void OnExit();

  boost::signals2::signal<void ()> onAccountsChanged;
  boost::signals2::signal<void (Account*)> onAccountEnabled;
  boost::signals2::signal<void (Account*)> onAccountDisabled;

 private:
  static const std::string kConfFile; /**< configuration file name */
  std::vector<Library> libraries_; /**< internal structure for available libraries */
  std::vector<Account*> accounts_; /**< internal structure for available accounts */

  /**
   * Boost serialization method.
   * Serializes DataManager internal structures.
   */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    //ar & boost::serialization::base_object<Properties>(*this);
    if (version) {}
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Properties);
    ar & BOOST_SERIALIZATION_NVP(libraries_);
    ar & BOOST_SERIALIZATION_NVP(accounts_);
  }
};

} // namespace sdc

#endif // CORE_DATA_MANAGER_H_
