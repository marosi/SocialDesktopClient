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
#include <QApplication>

namespace sdc {

class MainWindow;
class QtSettingsController;
class QtServiceModel;

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

  void Init();

  int Exec() {
    return app_.exec();
  }

 private:
  QApplication app_;
  MainWindow* main_view_;

 /**
  * Qt compatibility core layer
  */
  void OnAccountsChanged() {
    emit accountsChanged();
  }
  void OnGuiPrepared() {
    emit guiPrepared();
  }
  void OnAccountActivated(AccountData* model);
  void OnAccountDeactivated(AccountData* model);

 signals:
  void accountsChanged();
  void guiPrepared();
  void accountActivated(QtServiceModel* account);
  void accountDeactivated(QtServiceModel* account);
};

} /* namespace sdc */

#endif /* QT_GUI_H_ */