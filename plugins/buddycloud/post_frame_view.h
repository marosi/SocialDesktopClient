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
#include <QtGui/QToolBar>
#include "ui_post_frame_view.h"
#include "log.h"

class PostFrameView : public QFrame {

    Q_OBJECT

 public:
  PostFrameView(QWidget* parent = 0) {
    ui_.setupUi(this);
    ui_.toolButton->hide();
    connect(ui_.toolButton, SIGNAL(clicked()),
        this, SLOT(DeletePost()));
    /*
     * Tool bar creation
     */
    tool_bar_ = new QToolBar(ui_.content);
    comment_ = tool_bar_->addAction("comment");
    show_comments_ = tool_bar_->addAction("show comments");
    PositionLowerToolbar();
    tool_bar_->hide();
  }

  void SetID(const QString &id) {
    id_ = id;
  }

  void SetAuthor(const QString &info) {
    ui_.author->setText(info);
  }

  void SetContent(const QString &content) {
    ui_.content->setText(content);
  }

  /*
   * Events
   */
  void enterEvent(QEvent* event) {
    ui_.toolButton->show();
    tool_bar_->show();
  }

  void leaveEvent(QEvent* event) {
    ui_.toolButton->hide();
    tool_bar_->hide();
  }

  void resizeEvent(QResizeEvent * event) {
    PositionLowerToolbar();
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
  void PositionLowerToolbar() {
    QRect r1 = tool_bar_->geometry();
    QRect r2 = ui_.content->geometry();
    tool_bar_->move(r2.width() - r1.width(), r2.height() - r1.height());
  }

  Ui::PostFrameView ui_;
  QString id_;
  /*
   * Post tool bar
   */
  QToolBar* tool_bar_;
  QAction* comment_;
  QAction* show_comments_;
};

#endif /* POST_FRAME_VIEW_H_ */
