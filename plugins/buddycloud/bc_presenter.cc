/*
 * bc_presenter.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 7, 2012
 */

#include "bc_presenter.h"
#include "bc_contact_widget.h"

void BcPresenter::Init() {
  LOG(DEBUG) << " BcPresenter thread ID: " << boost::this_thread::get_id();
  model_ = model<BcModel>();
  model_->onConnected.connect([&] () {
    LOG(DEBUG) << " Direct signal - thread ID: " << boost::this_thread::get_id();
  });

  account_button()->menu()->addAction("Cuuuuz");

  sdc::bind(model_->onConnected, [&] () {
    LOG(DEBUG) << " In Cuz() thread ID: " << boost::this_thread::get_id();
    account_button()->menu()->addAction("123213213");
  });

  sdc::bind(model_->onContactsReady, boost::bind(&BcPresenter::PopulateContactPanel, this, _1));
}

void BcPresenter::PopulateContactPanel(const std::vector<BcContact::Ref> contacts) {
  LOG(DEBUG) << " In PopulateContactPanel() thread ID: " << boost::this_thread::get_id();
  for (const BcContact::Ref &contact : contacts) {
    BcContactWidget* cw = new BcContactWidget(main_window(), contact);
    main_window()->AddContact(this, cw);
  }
}
