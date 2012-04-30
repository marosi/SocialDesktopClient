/**
 * @file bc_contact_widget.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 24, 2012
 */

#ifndef BC_CONTACT_WIDGET_H_
#define BC_CONTACT_WIDGET_H_

#include "sdc.h"
#include "bc_contact.h"

class BcContactWidget : public sdc::ContactWidget {
 public:
  BcContactWidget(QWidget* parent, BcContact::Ref contact)
      : sdc::ContactWidget(parent, contact), contact_(contact) {

  }

  void Remove() {
    contact_->bot()->RemoveContact(contact_);
  }

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) {
    LOG(DEBUG) << "Clicking contact!";
    event->ignore();
    contact_->bot()->GetContactChannel(contact_);
  }

 private:
  BcContact::Ref contact_;
};

#endif /* BC_CONTACT_WIDGET_H_ */
