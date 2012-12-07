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

/**
 * Common class for specific social network new contact widgets.
 */
class NewContactWidget : public QWidget {
 public:
  /**
   * Verifies input validity.
   * @return true if input data is valid, false otherwise
   */
  virtual bool IsInputValid()=0;
  /**
   * Handler for processing new contact within its ServiceModel.
   * @param model service model
   */
  virtual void Process(ServiceModel* model)=0;
  /**
   * Gets error message.
   * @return error message
   */
  QString GetErrorMessage() {
    return error_;
  }
 protected:
  /**
   * Sets error message.
   * @param msg
   */
  void SetErrorMessage(const QString &msg) {
    error_ = msg;
  }
 private:
  QString error_;
};

} // namespace sdc

#endif // NEW_CONTACT_WIDGET_H_
