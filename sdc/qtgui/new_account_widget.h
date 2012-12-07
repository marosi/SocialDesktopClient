/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef NEW_ACCOUNT_WIDGET_H_
#define NEW_ACCOUNT_WIDGET_H_

#include <QWidget>

namespace sdc {

class Account;

/**
 * Common class for service specific new account widgets.
 */
class NewAccountWidget : public QWidget {
 public:
  /**
   * Handler method for setting account data from internal widgets form.
   * @param account account instance
   */
  virtual void BindDataTo(Account* account)=0;
  /**
   * Handler for settings internal widgets form from provided account data.
   * @param account account instance
   */
  virtual void BindDataFrom(const Account* account)=0;
  /**
   * Factory method for creating new service account.
   * @return new account instance
   */
  virtual Account* CreateNew()=0;
  /**
   * Handler method for telling, whether the filled form fields is valid or not.
   * @return true if data for new account are valid, false otherwise
   */
  virtual bool IsDataValid()=0;

  /**
   * Gets error message, in case something went wrong.
   * @return error message
   */
  QString GetErrorMessage() {
    return error_message_;
  }

 protected:
  /**
   * Sets error message, in case something went wrong.
   * @param msg error message
   */
  void SetErrorMessage(const QString &msg) {
    error_message_ = msg;
  }

 private:
  QString error_message_;
};

} // namespace sdc

#endif // NEW_ACCOUNT_WIDGET_H_
