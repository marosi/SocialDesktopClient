/*
 * qt_gui.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 2, 2012
 */

#include "bind.h"
#include "qt_gui.h"
#include "main_window.h"
#include "connection_manager.h"
#include "config_manager.h"
#include "contact_widget.h"
#include "content.h"
#include "service_model.h"
#include "service_presenter.h"
#include "qt_service_model.h"
#include "widget_factory.h"
#include "generic_widget_factory.h"
#include "boost/foreach.hpp"
#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include <vector>
#include <QSettings>

using namespace boost;
using std::vector;

namespace sdc {

//class ContactWidgetFactory : public GenericWidgetFactory<Contact, ContactWidget> {};

QtGui::~QtGui() {
  QSettings settings;
  //settings.setValue("sample", 123);
//  for (WidgetFactory* factory : factories_) {
//    delete factory;
//  }
}

void QtGui::Init() {
  // Init core models, controllers and views
  UI::Init();

  /*
   * Register core class for Qt meta processing
   * NOTE: For the type to be handled by QVariant it needs to be
   * added by macro Q_DECLARE_METATYPE(TypeName)
   */
  //qRegisterMetaType<Content::Ref>("Content::Ref");
  qRegisterMetaType<bind_params_base*>("bind_params_base");
  /*
   * Connect core signals
   */
  core()->data()->onAccountsChanged.connect(
      [&] () {
        emit accountsChanged();
      });
  core()->onGuiPrepared.connect(
      [&] () {
        emit guiPrepared();
      });
  core()->onAccountActivated.connect(
      [&] (AccountData* account) {
        QtServiceModel* model = dynamic_cast<QtServiceModel*>(account->GetServiceModel());
        if (model) {
          model->SetGui(this);
          emit accountActivated(model);
        }
      });
  core()->onAccountDeactivated.connect(
      [&] (AccountData* account) {
        emit accountDeactivated(account);
      });
//  core()->onContentView.connect(
//      [&] (Content::Ref content) {
//        emit contentView(content);
//      });
  connect(this, SIGNAL(accountActivated(QtServiceModel*)),
      this, SLOT(OnAccountActivated(QtServiceModel*)));
//  connect(this, SIGNAL(contentView(Content::Ref)),
//      this, SLOT(HandleContentView(Content::Ref)));
  /*
   * Qt settings for storing GUI state
   */
  QCoreApplication::setOrganizationName("sdc");
  QCoreApplication::setOrganizationDomain("socialdesktopclient.net");
  QCoreApplication::setApplicationName("Social Desktop Client");

  QSettings settings;
  //LOG(DEBUG) << "QSettings testing :: " << settings.value("sample").toInt();

  /*
   * Add default widget factories and widget factories from service plugins
   */
  //factories_.push_back(new ContactWidgetFactory);

//  for (Service* service : core()->services()) {
//    QtService* s = dynamic_cast<QtService*>(service);
//    if (s) {
//      vector<WidgetFactory*> f = s->CreateWidgetFactories();
//      factories_.insert(factories_.end(), f.begin(), f.end());
//    }
//  }
  /*
   * Main window initialization
   */
  main_view_ = new MainWindow(this);

  /*
   * signal/slot connections
   */
  connect(this, SIGNAL(accountDeactivated(AccountData*)),
        main_view_, SLOT(DeactivateAccount(AccountData*)));

  /*
   * Show main window finally
   */
  main_view_->show();
}

//QWidget* QtGui::CreateContentWidget(Content::Ref content, QWidget* parent) {
//  vector<WidgetFactory*>::reverse_iterator it;
//  QWidget* widget = 0;
//  for (it = factories_.rbegin(); it != factories_.rend(); ++it) {
//    if ((*it)->CanCreate(content)) {
//      // create widget for this specific content
//      if (parent)
//        widget = (*it)->Create(parent, content);
//      else
//        widget = (*it)->Create(main_view_, content);
//      content_widgets_.push_back(widget);
//      // No more widget can be created
//      break;
//    }
//  }
//  assert(widget);
//  return widget;
//}

void QtGui::OnAccountActivated(QtServiceModel* model) {
  ServicePresenter* presenter = model->GetQtService()->CreateServicePresenter();
  presenter->main_window_ = main_view_;
  presenter->service_model_ = model;
  // add mapping for model/presenter/service
  model_to_presenter_[model] = presenter;
  presenter_to_model_[presenter] = model;
  presenter_to_service_[presenter] = model->GetQtService();
  // activate account in main window
  main_view_->ActivateAccount(model);
}

//void QtGui::HandleContentView(Content::Ref content) {
//  this->CreateContentWidget(content);
//}

} /* namespace sdc */
