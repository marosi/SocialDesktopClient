/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef NEW_CONTACT_WIDGET_H_
#define NEW_CONTACT_WIDGET_H_

#include <QtGui/QWidget>

namespace sdc {

class ServiceModel;

class NewContactWidget : public QWidget {
 public:
  virtual bool IsInputValid()=0;
  virtual void Process(ServiceModel* model)=0;
  QString GetErrorMessage() {
    return error_;
  }
 protected:
  void SetErrorMessagte(const QString &msg) {
    error_ = msg;
  }
 private:
  QString error_;
};

} // namespace sdc

#endif // NEW_CONTACT_WIDGET_H_
