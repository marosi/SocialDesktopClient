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
#include "channel_widget.h"
#include "bc_contact.h"
// includes from sdc
#include "main_window.h"

class BcContactWidget : public sdc::ContactWidget {
 public:
  BcContactWidget(sdc::MainWindow* parent, BcContact::Ref contact)
      : sdc::ContactWidget(parent, contact),
        window_(parent),
        contact_(contact),
        channel_(0) {

  }

  void Remove() {
    // TODO: impelment
    //contact_->bot()->RemoveContact(contact_);
  }

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) {
    LOG(DEBUG) << "Clicking contact!";
    event->ignore();
    ShowChannelPanel();
  }

 private:
  void ShowChannelPanel() {
    LOG(DEBUG) << " In ShowChannelPanel() thread ID: " << boost::this_thread::get_id();
    if (!channel_) {
      channel_ = new ChannelWidget(contact_->GetChannel());
    }
    window_->AddContentPanel(channel_);
    channel_->show();
  }

  sdc::MainWindow* window_;
  BcContact::Ref contact_;
  ChannelWidget* channel_;
};

#endif /* BC_CONTACT_WIDGET_H_ */
