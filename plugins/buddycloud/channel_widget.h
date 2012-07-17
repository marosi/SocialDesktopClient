/**
 * @file channel_widget.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 23, 2012
 */

#ifndef CHANNEL_WIDGET_H_
#define CHANNEL_WIDGET_H_

#include "abstract_presenter.h"
#include "content_panel.h"
#include "channel.h"
#include "ui_new_post_widget.h"
#include "boost/date_time/posix_time/ptime.hpp"
#include <QList>
#include <QMap>

class PostWidget;

class ChannelWidget : public sdc::ContentPanel, public AbstractPresenter {

    Q_OBJECT

 public:
  ChannelWidget(AbstractPresenter* presenter, ChannelController* channel);
  ~ChannelWidget();

 public slots:
  void SendPost();
  void OnScrollBarValueChanged(int value);

 private:
  void ShowPostInOrder(Post1* post);

  ChannelController* channel_;
  QList<Post1*> posts_order_;
  QMap<Post1*, PostWidget*> posts_;
  QToolButton* new_post_button_;
  QWidget* new_post_;
  Ui::NewPostClass new_post_ui;
  QScrollBar* scroll_bar_;
  int old_scroll_bar_value_;
};

#endif /* CHANNEL_WIDGET_H_ */
