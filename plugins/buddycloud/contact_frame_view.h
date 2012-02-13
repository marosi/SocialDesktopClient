/**
 * @file contact_frame.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 11, 2012
 */

#ifndef CONTACT_FRAME_H_
#define CONTACT_FRAME_H_

#include <QtGui/QFrame>
#include "ui_contact_frame_view.h"

class ContactFrameView : public QFrame {

    Q_OBJECT

 public:
  ContactFrameView(QWidget* parent = 0) {
    frame.setupUi(this);
  }

  void SetInfo(const QString &info) {
    frame.contactInfo->setText(info);
  }

 private:
  Ui::ContactFrameView frame;
};

#endif /* CONTACT_FRAME_H_ */
