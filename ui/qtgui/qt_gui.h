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
class WidgetFactory;

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

  void Init();

  int Exec() {
    return app_.exec();
  }

 private:
  QApplication app_;
  MainWindow* main_view_;
  std::vector<WidgetFactory*> factories_;
  std::vector<QWidget*> content_widgets_;

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
  void OnContentView(Content::Ref content) {
    emit contentView(content);
  }

 private slots:
  void HandleContentView(Content::Ref content);

 signals:
  void accountsChanged();
  void guiPrepared();
  void accountActivated(QtServiceModel* account);
  void accountDeactivated(AccountData* account);
  void contentView(Content::Ref content);
};

} /* namespace sdc */

#endif /* QT_GUI_H_ */
