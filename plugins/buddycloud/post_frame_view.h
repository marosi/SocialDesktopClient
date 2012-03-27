/**
 * @file post_frame_view.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 26, 2012
 */

#ifndef POST_FRAME_VIEW_H_
#define POST_FRAME_VIEW_H_

#include <QtGui/QFrame>
#include "ui_post_frame_view.h"

class PostFrameView : public QFrame {

    Q_OBJECT

 public:
  PostFrameView(QWidget* parent = 0) {
    frame.setupUi(this);
  }

  void SetAuthor(const QString &info) {
    frame.author->setText(info);
  }

  void SetContent(const QString &content) {
    frame.content->setText(content);
  }

 private:
  Ui::PostFrameView frame;
};

#endif /* POST_FRAME_VIEW_H_ */
