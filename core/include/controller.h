/**
 * @file controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 6, 2011
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <QObject>

namespace sdc {

class Controller : public QObject {

  Q_OBJECT

 public:
  /*void SetView(QWidget* view) {
    view_ = view;
  }
  QWidget* GetView() {
    return view_;
  }*/
  void SignalIt(std::string content);

 public slots:
 signals:
  void update(QString content);
};

} /* namespace sdc */

#endif /* CONTROLLER_H_ */
