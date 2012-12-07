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

/**
 * Common widget for activity notification of any social network service.
 */
class Activity : public QFrame {

    Q_OBJECT
    
 public:
  explicit Activity(QFrame *parent = 0);
  virtual ~Activity();

  /**
   * Gets label image.
   * @return label
   */
  QLabel* image_label();
  /**
   * Gets text label right next to the image label.
   * @return text label
   */
  QLabel* text_label();

 private:
  Ui::Activity *ui;
};

} // namespace sdc

#endif // ACTIVITY_H_
