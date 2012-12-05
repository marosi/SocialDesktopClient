/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "discover_service.h"
#include "pubsub_requests.h"
#include "sdc/core/log.h"
#include "Swiften/Disco/GetDiscoInfoRequest.h"
#include "Swiften/Disco/GetDiscoItemsRequest.h"
#include "boost/bind.hpp"
#include "boost/foreach.hpp"

using namespace Swift;
using namespace boost;
using std::string;
using std::vector;

void DiscoverService::Discover(const JID &jid, IQRouter* router) {
  jid_ = jid;
  router_ = router;
  GetDiscoItemsRequest::ref items = GetDiscoItemsRequest::create(jid_.getDomain(), router_);
  items->onResponse.connect([&] (shared_ptr<DiscoItems> payload, ErrorPayload::ref error) {
    if (error) {
      LOG(ERROR) << error->getText();
      return;
    }
    if (payload->getItems().size() <= 0) {
      onError(ChannelsServiceUnavailable);
    } else {
      LOG(TRACE) << "Channel " << jid_ << " : handling domain disco items ... ";
      vector<DiscoItems::Item>::const_iterator it = payload->getItems().begin();
      GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), router_);
      info->onResponse.connect(bind(&DiscoverService::handleDomainItemInfo, this, _1, _2, payload, it));
      info->send();
    }
  });
  items->send();
}

void DiscoverService::handleDomainItemInfo(DiscoInfo::ref payload, ErrorPayload::ref error, shared_ptr<DiscoItems> items, vector<DiscoItems::Item>::const_iterator it) {
  if (error) {
    LOG(ERROR) << error->getText();
  } else {
    BOOST_FOREACH (const DiscoInfo::Identity &identity , payload->getIdentities()) {
      // Is channel service?
      if (identity.getCategory() == "pubsub" && identity.getType() == "channels") {
        service_.is_available = true;
        service_.jid = it->getJID();
        if (payload->hasFeature("jabber:iq:register")) {
          service_.is_registration_available = true;
        } else {
          service_.is_registration_available = false;
        }
        // Signal channel available
        onAvailable(service_, jid_);
      }
    }
  }
  if (!service_.is_available) { // Go for next item if service is not found yet
    ++it;
    if (it == items->getItems().end()) {
      LOG(TRACE) << "Channel service of domain " << jid_.getDomain() << " is not available!";
      onError(ChannelsServiceUnavailable);
    } else {
      GetDiscoInfoRequest::ref info = GetDiscoInfoRequest::create(it->getJID(), router_);
      info->onResponse.connect(bind(&DiscoverService::handleDomainItemInfo, this, _1, _2, items, it));
      info->send();
    }
  }
}
