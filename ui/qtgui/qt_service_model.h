/**
 * @file qt_service_model.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 14, 2012
 */

#ifndef QT_SERVICE_MODEL_H_
#define QT_SERVICE_MODEL_H_

#include "service_model.h"
#include <QObject>

namespace sdc {

class QtServiceModel : public QObject, public ServiceModel {

    Q_OBJECT

 public:
  QtServiceModel(AccountData* account) : ServiceModel(account) {}

 private:
};

} /* namespace sdc */

#endif /* QT_SERVICE_MODEL_H_ */
