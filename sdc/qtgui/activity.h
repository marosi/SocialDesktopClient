#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class Activity;
}

namespace sdc {

class Activity : public QWidget {

    Q_OBJECT
    
 public:
  explicit Activity(QWidget *parent = 0);
  virtual ~Activity();

  QLabel* image_label();
  QLabel* text_label();

 private:
  Ui::Activity *ui;
};

} /* namespace sdc */

#endif // ACTIVITY_H
