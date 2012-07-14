#include "bc_contact_widget.h"
#include "channel_widget.h"
#include "bc_contact.h"

#include "bind.h"
#include <QMouseEvent>

BcContactWidget::BcContactWidget(sdc::MainWindow* parent, BcContact* contact)
    : sdc::ContactWidget(parent, contact),
      window_(parent),
      contact_(contact),
      channel_(0) {
  sdc::bind(contact_->onAvatarChanged, [&] () {
    contact_image_button()->setIcon(QIcon(QString::fromStdString(contact_->GetAvatarPath())));
  });
}

void BcContactWidget::mouseDoubleClickEvent(QMouseEvent* event) {
  LOG(DEBUG) << "Clicking contact!";
  event->ignore();
  ShowChannelPanel();
}

void BcContactWidget::ShowChannelPanel() {
  if (!channel_) {
    channel_ = new ChannelWidget(contact_->GetChannel());
  }
  window_->AddContentPanel(channel_);
  channel_->show();
}
