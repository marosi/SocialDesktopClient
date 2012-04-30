/**
 * @file new_contact_widget.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 24, 2012
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

} /* namespace sdc */

#endif /* NEW_CONTACT_WIDGET_H_ */
