/**
 * @file qt_gui.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef QT_GUI_H_
#define QT_GUI_H_

#include "ui.h"
#include "content.h"
#include <QApplication>

namespace sdc {

class MainWindow;
class QtSettingsController;
class QtServiceModel;
class QtService;
class WidgetFactory;
class ServicePresenter;

/**
 * Kind of a main controller in MVC abstraction and also Manager in Core abstraction.
 */
class QtGui : public QObject, public UI {

    Q_OBJECT

 public:
  QtGui(Core* core, int argc, char* argv[])
      : UI(core),
        app_(argc, argv),
        main_view_(0) {}
  ~QtGui();

  void Init(); // TODO: do something with init() and exec() they are accessible from each QtView

  int Exec() {
    return app_.exec();
  }

  QtServiceModel* GetModel(ServicePresenter* presenter) {
    return presenter_to_model_[presenter];
  }

  ServicePresenter* GetPresenter(QtServiceModel* model) {
    return model_to_presenter_[model];
  }

  QtService* GetService(ServicePresenter* presenter) {
    return presenter_to_service_[presenter];
  }

  //QWidget* CreateContentWidget(Content::Ref, QWidget* parent = 0);

 signals:
  void accountsChanged();
  void guiPrepared();
  void accountActivated(QtServiceModel* account);
  void accountDeactivated(AccountData* account);
  //void contentView(Content::Ref content);

 private:
  QApplication app_;
  MainWindow* main_view_;
  std::vector<WidgetFactory*> factories_;
  std::vector<QWidget*> content_widgets_;
  std::map<QtServiceModel*, ServicePresenter*> model_to_presenter_;
  std::map<ServicePresenter*, QtServiceModel*> presenter_to_model_;
  std::map<ServicePresenter*, QtService*> presenter_to_service_;

 private slots:
  //void HandleContentView(Content::Ref content);
  void OnAccountActivated(QtServiceModel* model);
};

} /* namespace sdc */

#endif /* QT_GUI_H_ */
