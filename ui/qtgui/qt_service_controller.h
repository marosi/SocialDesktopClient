/**
 * @file qt_service_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef QT_SERVICE_CONTROLLER_H_
#define QT_SERVICE_CONTROLLER_H_

#include "service_controller.h"
#include "qt_controller.h"
#include <QtGui/QWidget>

namespace sdc {

class QtServiceController : public ServiceController, public QtController {
};

} /* namespace sdc */

#endif /* QT_SERVICE_CONTROLLER_H_ */
