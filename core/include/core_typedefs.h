/**
 * @file core_typedefs.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 7, 2012
 */

#ifndef CORE_TYPEDEFS_H_
#define CORE_TYPEDEFS_H_

#include "boost/shared_ptr.hpp"

namespace sdc {

class Connection;
class ServiceModel;
class ServiceView;
class ServiceController;

typedef boost::shared_ptr<Connection> ConnectionRef;
typedef boost::shared_ptr<ServiceModel> ServiceModelRef;
typedef boost::shared_ptr<ServiceView> ServiceViewRef;
typedef boost::shared_ptr<ServiceController> ServiceControllerRef;

}  /* namespace sdc */

#endif /* CORE_TYPEDEFS_H_ */
