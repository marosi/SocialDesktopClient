/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef QT_SERVICE_MODEL_H_
#define QT_SERVICE_MODEL_H_

#include "service_model.h"
#include "contact.h"
#include "qt_service.h"
#include <QObject>

namespace sdc {

class QtGui;

/**
 * Qt ServiceModel class.
 * Servic model conforming to the requirements of Qt GUI.
 */
class QtServiceModel : public QObject, public ServiceModel {
 public:
  QtServiceModel(Account* account);
  virtual ~QtServiceModel() {}

  /**
   * Set main Qt GUI controller.
   * @param gui qt gui controller
   */
  void SetGui(QtGui* gui) {
    gui_ = gui;
  }
  QtService* GetQtService();

 protected:
  /**
   * Gets main Qt GUI controller.
   * @return Qt GUI controller
   */
  QtGui* GetGui() {
    return gui_;
  }
 private:
  QtGui* gui_;
};

} // namespace sdc

#endif // QT_SERVICE_MODEL_H_
