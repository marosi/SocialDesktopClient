/**
 * @file posts_frame_header.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 28, 2012
 */

#ifndef POSTS_FRAME_HEADER_H_
#define POSTS_FRAME_HEADER_H_

#include <QtGui/QFrame>
#include <QtGui/QToolBar>
#include "ui_posts_frame_view.h"
#include "post_frame_view.h"
#include "sdc.h"

class PostsFrameView : public QFrame {

    Q_OBJECT

 public:
  PostsFrameView(QObject* controller, QWidget* parent = 0) {
    controller_ = controller;
    ui_.setupUi(this);
    parent->layout()->addWidget(this);
    /*
     * Toolbar
     */
    //tool_bar_ = new QToolBar(this);
    //new_post_ = tool_bar_->addAction("new post");
    ui_.upperToolBar->hide();
    ui_.newPostGroupBox->hide();
    connect(ui_.newPostToolButton, SIGNAL(toggled(bool)),
        ui_.newPostGroupBox, SLOT(setVisible(bool)));
    connect(ui_.sendPostToolButton, SIGNAL(clicked()),
        this, SLOT(SendNewPost()));
    /*
     * Controller actions
     */
     connect(this, SIGNAL(signalNewPost(QString)),
         controller_, SLOT(SendNewPost(QString)));
  }

  /*
   * Events
   */
  void enterEvent(QEvent* event) {
    //tool_bar_->show();
    ui_.upperToolBar->show();
    ui_.upperToolBar->raise();
  }

  void leaveEvent(QEvent* event) {
    //tool_bar_->hide();
    ui_.upperToolBar->hide();
  }

 public slots:
  void ShowPost(const QString &id, const QString &author, const QString &content) {
    LOG(DEBUG) << "showing post ..." << id.toStdString();
    PostFrameView* pframe = new PostFrameView(this);
    pframe->SetID(id);
    pframe->SetAuthor(author);
    pframe->SetContent(content);
    pframe->setEnabled(true);
    pframe->setVisible(true);
    connect(pframe, SIGNAL(signalDeletePost(QString)),
        controller_, SLOT(DeletePost(QString)));
    posts_.append(pframe);
    ui_.contentPanelLayout->addWidget(pframe);
  }

  void SendNewPost() {
    QString content = ui_.newPostTextEdit->toPlainText();
    if (content.isEmpty())
      return;
    emit signalNewPost(content);
    ui_.newPostTextEdit->clear();
  }

 signals:
  void signalNewPost(QString post);

 private:
  QObject* controller_; // TODO: anchor on controller NEEDED => put in base class
  Ui::PostsFrameView ui_;
  QList<PostFrameView*> posts_;
  QToolBar* tool_bar_;
  QAction* new_post_;
};

#endif /* POSTS_FRAME_HEADER_H_ */
