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

 private:
  QObject* controller_; // TODO: anchor on controller NEEDED => put in base class
  Ui::PostsFrameView ui_;
  QList<PostFrameView*> posts_;
};

#endif /* POSTS_FRAME_HEADER_H_ */
