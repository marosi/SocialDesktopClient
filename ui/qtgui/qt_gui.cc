/*
 * qt_gui.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 2, 2012
 */

#include "qt_gui.h"
#include "mainwindow.h"
#include "connection_manager.h"
#include "config_manager.h"
#include "content.h"
#include "qt_service_model.h"
#include "widget_factory.h"
#include "generic_widget_factory.h"
#include "boost/foreach.hpp"
#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include <vector>

using namespace boost;
using std::vector;

namespace sdc {

class ContactWidgetFactory : public GenericWidgetFactory<Contact, ContactWidget> {};

QtGui::~QtGui() {
  QSettings settings;
  //settings.setValue("sample", 123);
  BOOST_FOREACH (WidgetFactory* factory, factories_) {
    delete factory;
  }
}

void QtGui::Init() {
  // Init core models, controllers and views
  UI::Init();

  /*
   * Register core class for Qt meta processing
   * NOTE: For the type to be handled by QVariant it needs to be
   * added by macro Q_DECLARE_METATYPE(TypeName)
   */
  qRegisterMetaType<Content::Ref>("Content::Ref");

  /*
   * Connect core signals
   */
  core()->data()->onAccountsChanged.connect(bind(&QtGui::OnAccountsChanged, this));
  core()->onGuiPrepared.connect(bind(&QtGui::OnGuiPrepared, this));
  core()->onAccountActivated.connect(bind(&QtGui::OnAccountActivated, this, _1));
  core()->onAccountDeactivated.connect(bind(&QtGui::OnAccountDeactivated, this, _1));
  core()->onContentView.connect(bind(&QtGui::OnContentView, this, _1));
  connect(this, SIGNAL(contentView(Content::Ref)),
      this, SLOT(HandleContentView(Content::Ref)));
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
  factories_.push_back(new ContactWidgetFactory);

  BOOST_FOREACH (Service* service, core()->services()) {
    QtService* s = dynamic_cast<QtService*>(service);
    if (s) {
      vector<WidgetFactory*> f = s->CreateWidgetFactories();
      factories_.insert(factories_.end(), f.begin(), f.end());
    }
  }
  /*
   * Main window initialization
   */
  main_view_ = new MainWindow(this);
  /*BOOST_FOREACH (const ServiceController* sc, GetControllers()) {
    const QtServiceController* qsc = boost::polymorphic_downcast<const QtServiceController*>(sc);
    main_view_->AddServiceWidget(qsc->GetView());
  }*/ // TODO: REMOVE there is no controllers

  /*
   * signal/slot connections
   */
  connect(this, SIGNAL(accountActivated(QtServiceModel*)),
      main_view_, SLOT(ActivateAccount(QtServiceModel*)));
  connect(this, SIGNAL(accountDeactivated(AccountData*)),
        main_view_, SLOT(DeactivateAccount(AccountData*)));

  /*
   * Activate enabled accounts in GUI
   */
  BOOST_FOREACH (ServiceModel* model, core()->GetModels()) {
    QtServiceModel* qt_model = boost::polymorphic_downcast<QtServiceModel*>(model);
    emit accountActivated(qt_model);
  }

  /*
   * Show main window finally
   */
  main_view_->show();
}

QWidget* QtGui::CreateContentWidget(Content::Ref content, QWidget* parent) {
  vector<WidgetFactory*>::reverse_iterator it;
  QWidget* widget = 0;
  for (it = factories_.rbegin(); it != factories_.rend(); ++it) {
    if ((*it)->CanCreate(content)) {
      // create widget for this specific content
      if (parent)
        widget = (*it)->Create(parent, content);
      else
        widget = (*it)->Create(main_view_, content);
      content_widgets_.push_back(widget);
      // No more widget can be created
      break;
    }
  }
  assert(widget);
  return widget;
}

void QtGui::OnAccountActivated(AccountData* account) {
  QtServiceModel* qt_model = dynamic_cast<QtServiceModel*>(account->GetServiceModel());
  if (qt_model)
    emit accountActivated(qt_model);
}
void QtGui::OnAccountDeactivated(AccountData* account) {
  emit accountDeactivated(account);
}
void QtGui::HandleContentView(Content::Ref content) {
  this->CreateContentWidget(content);
}

} /* namespace sdc */
