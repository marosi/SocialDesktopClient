/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ACTIVITY_H_
#define ACTIVITY_H_

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

} // namespace sdc

#endif // ACTIVITY_H_
