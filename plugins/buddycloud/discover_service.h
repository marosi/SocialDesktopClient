/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef DISCOVER_SERVICE_H
#define DISCOVER_SERVICE_H

#include "Swiften/Elements/DiscoInfo.h"
#include "Swiften/Elements/DiscoItems.h"
#include "Swiften/Elements/ErrorPayload.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Queries/IQRouter.h"
#include "boost/shared_ptr.hpp"
#include "boost/signals2/signal.hpp"
#include <vector>

class DiscoverService {
 public:
  struct Info {
    Swift::JID jid;
    bool is_available;
    bool is_registration_available;

    Info(
        bool is_avail = false,
        bool is_registration = false)
        : is_available(is_avail),
          is_registration_available(is_registration) {}
  };

  enum Error {
    ChannelsServiceUnavailable,
    UserChannelNotPresent
  };

  void Discover(const Swift::JID &jid, Swift::IQRouter* router);

  boost::signals2::signal<void (Error)> onError;
  boost::signals2::signal<void (Info, Swift::JID)> onAvailable;

 private:
  void handleDomainItemInfo(Swift::DiscoInfo::ref payload, Swift::ErrorPayload::ref error, boost::shared_ptr<Swift::DiscoItems> items, std::vector<Swift::DiscoItems::Item>::const_iterator it);

  Swift::JID jid_;
  Swift::IQRouter* router_;
  Info service_;
};

#endif // DISCOVER_SERVICE_H
