#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class Activity;
}

namespace sdc {

class Activity : public QFrame {

    Q_OBJECT
    
 public:
  explicit Activity(QFrame *parent = 0);
  virtual ~Activity();

  QLabel* image_label();
  QLabel* text_label();

 private:
  Ui::Activity *ui;
};

} /* namespace sdc */

#endif // ACTIVITY_H
