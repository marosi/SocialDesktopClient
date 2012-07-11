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
#include "content.h"
#include "qt_service.h"
#include <QObject>

namespace sdc {

class QtGui;

class QtServiceModel : public QObject, public ServiceModel {
 public:
  QtServiceModel(AccountData* account);
  virtual ~QtServiceModel() {}

  void SetGui(QtGui* gui) {
    gui_ = gui;
  }
  QtService* GetQtService();

 protected:
  QtGui* GetGui() {
    return gui_;
  }
 private:
  QtGui* gui_;
};

} /* namespace sdc */

#endif /* QT_SERVICE_MODEL_H_ */
