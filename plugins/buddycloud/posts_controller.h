/**
 * @file posts_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 27, 2012
 */

#ifndef POSTS_CONTROLLER_H_
#define POSTS_CONTROLLER_H_

#include "sdc.h"
#include "buddycloud_connection.h"
#include "swift_commands.h"
#include "posts_frame_view.h"
#include "post_frame_view.h"
#include <vector>

class PostsController : public QObject, public sdc::Controller {

    Q_OBJECT

 public:
  PostsController(sdc::ServiceController* service_controller = 0) : parent_view_(0), view_(0), posts_(new sdc::Items<sdc::Post>) {
    service_controller_ = service_controller;
  }
  /*
   *  View creation method. TODO: They should go to parent class.
   */
  void CreateView(QWidget* parent) {
    parent_view_ = parent;
    connect(this, SIGNAL(signalCreateView(QObject*)),
        parent_view_, SLOT(CreatePostsView(QObject*)));
    connect(parent_view_, SIGNAL(signalPostsViewCreated(PostsFrameView*)),
            this, SLOT(HandleViewCreated(PostsFrameView*)));
    emit signalCreateView(this); // one time view creation;
    disconnect(this, SIGNAL(signalCreateView(QObject*)),
        parent_view_, SLOT(CreatePostsView(QObject*)));
    disconnect(parent_view_, SIGNAL(signalPostsViewCreated(PostsFrameView*)),
            this, SLOT(HandleViewCreated(PostsFrameView*)));
  }

  void ShowPosts(sdc::Items<sdc::Post>::Ref posts) {
    posts_->Clear();
    posts_ = posts;
    posts_->Iterate();
    while(sdc::Post::Ref post = posts->GetNext()) {
      LOG(DEBUG) << "posts controller displaying post";
      AppendPost(post);
    }
  }

  void AppendPost(sdc::Post::Ref post) {
    emit signalShowPost(
          QString::fromStdString(post->GetID()),
          QString::fromStdString(post->GetAuthor()),
          QString::fromStdString(post->GetContent()));
  }


 public slots:
  void DeletePost(QString id) {
    SwiftDeletePostRequest::Ref req(new SwiftDeletePostRequest(id.toStdString()));
    service_controller_->GetConnection<BuddycloudConnection>()->Send(req);
  }
  /*
   * TODO: Should be in every controller
   */
  void HandleViewCreated(PostsFrameView* widget) {
    view_ = widget;
    connect(this, SIGNAL(signalShowPost(const QString &, const QString &, const QString &)),
        view_, SLOT(ShowPost(const QString &, const QString &, const QString &)));
  }

  void SendNewPost(QString content) {
    sdc::Post::Ref post(new sdc::Post);
    post->SetAuthor("Pisticek");
    post->SetContent(content.toStdString());
    SwiftSendPostRequest::Ref req(new SwiftSendPostRequest(post));
    service_controller_->GetConnection<BuddycloudConnection>()->Send(req);
  }

 signals:
  void signalCreateView(QObject*); // TODO: probably goes to base class
  void signalShowPost(const QString &id, const QString & author, const QString & content);

 private:
  sdc::ServiceController* service_controller_;
  QWidget* parent_view_;
  PostsFrameView* view_;
  sdc::Items<sdc::Post>::Ref posts_;
};

#endif /* POSTS_CONTROLLER_H_ */
