/**
 * @file qt_gui.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */
/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef QT_GUI_H_
#define QT_GUI_H_

#include "ui.h"
#include <QApplication>
#include <QString>
#include <map>
#include <vector>

namespace sdc {

class Account;
class AccountButton;
class MainWindow;
class QtSettingsController;
class QtServiceModel;
class QtService;
class WidgetFactory;
class ServicePresenter;

/**
 * Qt GUI implementations for SocialDesktopClient.
 *
 * This class is instatiated along with the Core object.
 * It is also a place to handle Core signals.
 */
class QtGui : public QObject, public UI {

    Q_OBJECT

 public:
  QtGui(int argc, char* argv[])
      : app_(argc, argv),
        main_window_(0) {}
  ~QtGui();

  /**
   * Called from Core. Initializes Qt GUI components.
   */
  void Init(); // TODO: do something with init() and exec() they are accessible from each QtView

  /**
   * Called from Core. Starts GUI application.
   * @return exit code
   */
  int Exec() {
    return app_.exec();
  }

  /**
   * Gets model based on associated presenter.
   * @param presenter service presenter
   * @return associated model
   */
  QtServiceModel* GetModel(ServicePresenter* presenter) {
    return presenter_to_model_[presenter];
  }

  /**
   * Gets presenter based on associated model.
   * @param model servic model
   * @return associated presenter
   */
  ServicePresenter* GetPresenter(QtServiceModel* model) {
    return model_to_presenter_[model];
  }

  /**
   * Gets service instance based on associated presenter.
   * @param presenter presenter
   * @return associated service
   */
  QtService* GetService(ServicePresenter* presenter) {
    return presenter_to_service_[presenter];
  }

  /**
   * Interface for applying stylesheets.
   * @param stylesheet
   */
  void AppendStyleSheet(const QString &stylesheet);

 private:
  void ActivateAccount(const std::string account_id);
  void DeativateAccount(const std::string account_id);

  QApplication app_;
  MainWindow* main_window_;
  std::vector<WidgetFactory*> factories_;
  std::vector<QWidget*> content_widgets_;
  std::map<std::string, AccountButton*> buttons_;
  std::map<std::string, ServicePresenter*> id_to_presenter_;
  std::map<QtServiceModel*, ServicePresenter*> model_to_presenter_;
  std::map<ServicePresenter*, QtServiceModel*> presenter_to_model_;
  std::map<ServicePresenter*, QtService*> presenter_to_service_;
};

} // namespace sdc

#endif // QT_GUI_H_
