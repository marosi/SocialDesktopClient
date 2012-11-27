/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "qt_service_model.h"
#include "qt_service.h"
#include "boost/cast.hpp"

namespace sdc {

QtServiceModel::QtServiceModel(Account* account) : ServiceModel(account) {

}

QtService* QtServiceModel::GetQtService() {
  return boost::polymorphic_downcast<QtService*>(this->service());
}

} // namespace sdc
