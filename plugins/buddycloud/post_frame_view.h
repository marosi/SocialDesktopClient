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
    frame.toolButton->hide();
    connect(frame.toolButton, SIGNAL(clicked()),
        this, SLOT(DeletePost()));
  }

  void SetID(const QString &id) {
    id_ = id;
  }

  void SetAuthor(const QString &info) {
    frame.author->setText(info);
  }

  void SetContent(const QString &content) {
    frame.content->setText(content);
  }

  /*
   * Events
   */
  void enterEvent(QEvent* event) {
    frame.toolButton->show();
  }

  void leaveEvent(QEvent* event) {
    frame.toolButton->hide();
  }

  /*
   * Actions
   */
 public slots:
  void DeletePost() {
    emit signalDeletePost(id_);
  }

 signals:
  void signalDeletePost(QString id);

 private:
  Ui::PostFrameView frame;
  QString id_;
};

#endif /* POST_FRAME_VIEW_H_ */
