/**
 * @file mvc_typedefs.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 31, 2012
 */

#ifndef CORE_MVC_TYPEDEFS_H_
#define CORE_MVC_TYPEDEFS_H_

#include "boost/shared_ptr.hpp"

namespace sdc {

class Model;
class View;
class Controller;

typedef boost::shared_ptr<Model> ModelRef;
typedef boost::shared_ptr<View> ViewRef;
typedef boost::shared_ptr<Controller> ControllerRef;

} /* namespace sdc */

#endif /* CORE_MVC_TYPEDEFS_H_ */
