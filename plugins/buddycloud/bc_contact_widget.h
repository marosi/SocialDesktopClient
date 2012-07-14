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

#include "contact_widget.h"
// includes from sdc
#include "main_window.h"

class BcContact;
class ChannelWidget;

class BcContactWidget : public sdc::ContactWidget {
 public:
  BcContactWidget(sdc::MainWindow* parent, BcContact* contact);

  void Remove() {
    // TODO: impelment
    //contact_->bot()->RemoveContact(contact_);
  }

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event);

 private:
  void ShowChannelPanel();

  sdc::MainWindow* window_;
  BcContact* contact_;
  ChannelWidget* channel_;
};

#endif /* BC_CONTACT_WIDGET_H_ */
