/**
 * @file new_account_widget.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 14, 2012
 */

#ifndef NEW_ACCOUNT_WIDGET_H_
#define NEW_ACCOUNT_WIDGET_H_

#include "account_data.h"
#include <QWidget>

namespace sdc {

class NewAccountWidget : public QWidget {
 public:
  virtual void BindDataTo(AccountData* account)=0;
  virtual void BindDataFrom(const AccountData* account)=0;
  virtual AccountData* CreateNew()=0;
  virtual bool IsDataValid()=0;

  QString GetErrorMessage() {
    return error_message_;
  }

 protected:
  void SetErrorMessage(const QString &msg) {
    error_message_ = msg;
  }

 private:
  QString error_message_;
};

} /* namespace sdc */

#endif /* NEW_ACCOUNT_WIDGET_H_ */
