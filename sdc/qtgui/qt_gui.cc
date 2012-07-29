/*
 * qt_gui.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 2, 2012
 */

#include "qt_gui.h"
#include "account_button.h"
#include "bind.h"
#include "core.h"
#include "main_window.h"
#include "service_model.h"
#include "service_presenter.h"
#include "qt_service_model.h"
#include <QFile>
#include <QSettings>
#include "boost/foreach.hpp"
#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include <vector>

namespace sdc {

QtGui::~QtGui() {
  QSettings settings;
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
   * Bind to core signals
   */
  sdc::bind(Core::Instance()->onAccountActivated, boost::bind(&QtGui::ActivateAccount, this, _1));
  sdc::bind(Core::Instance()->onAccountDeactivated, boost::bind(&QtGui::DeativateAccount, this, _1));
  /*
   * Qt settings for storing GUI state
   */
  QCoreApplication::setOrganizationName("SDC");
  QCoreApplication::setApplicationName("Social Desktop Client");
  //QSettings settings;
  //LOG(DEBUG) << "QSettings testing :: " << settings.value("sample").toInt();
  /*
   * Read stylesheet from file
   */
  QFile file("sdc/qtgui/resources/sdc.qss");
  file.open(QFile::ReadOnly);
  QString stylesheet = QLatin1String(file.readAll());
  app_.setStyleSheet(stylesheet);
  /*
   * Main window creation
   */
  main_window_ = new MainWindow(this);
  /*
   * Show main window finally
   */
  main_window_->show();
}

void QtGui::AppendStyleSheet(const QString &stylesheet) {
  app_.setStyleSheet(app_.styleSheet() + stylesheet);
}

void QtGui::ActivateAccount(const std::string account_id) {
  QtServiceModel* model = boost::polymorphic_downcast<QtServiceModel*>(Core::Instance()->model(account_id));
  // create presenter
  ServicePresenter* presenter = model->GetQtService()->CreateServicePresenter();
  // create account button
  AccountButton* button = new AccountButton(model);
  main_window_->AddAccountButton(button);
  buttons_[account_id] = button;
  // initialize presenter members
  presenter->qtgui_ = this;
  presenter->account_button_ = button;
  presenter->main_window_ = main_window_;
  presenter->service_model_ = model;
  // add mapping for model/presenter/service
  id_to_presenter_[account_id] = presenter;
  model_to_presenter_[model] = presenter;
  presenter_to_model_[presenter] = model;
  presenter_to_service_[presenter] = model->GetQtService();
  // finally initialize presenter
  presenter->Init();
}

void QtGui::DeativateAccount(const std::string account_id) {
  ServicePresenter* presenter = id_to_presenter_[account_id];
  QtServiceModel* model = presenter_to_model_[presenter];
  // remove all widgets parented by this accout
  main_window_->RemoveAccountButton(buttons_[account_id]);
  main_window_->RemoveAllContacts(presenter);
  main_window_->RemoveAllContentPanels(presenter);
  buttons_.erase(account_id);
  model_to_presenter_.erase(model);
  presenter_to_model_.erase(presenter);
  presenter_to_service_.erase(presenter);
  delete presenter;
}

} /* namespace sdc */
