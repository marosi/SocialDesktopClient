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

class QtServiceModel : public QObject, public ServiceModel {
 public:
  QtServiceModel(Account* account);
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

} // namespace sdc

#endif // QT_SERVICE_MODEL_H_
