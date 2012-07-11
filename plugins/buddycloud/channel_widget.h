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

#include "content_panel.h"
#include "channel.h"
#include "ui_new_post_widget.h"
#include <QList>
#include <string>

class PostWidget;

class ChannelWidget : public sdc::ContentPanel {

    Q_OBJECT

 public:
  ChannelWidget(ChannelController* channel);

 public slots:
  void SendPost();

 private:
  ChannelController* channel_;
  QMap<std::string, PostWidget*> posts_;
  QToolButton* new_post_button_;
  QWidget* new_post_;
  Ui::NewPostClass new_post_ui;
};

#endif /* CHANNEL_WIDGET_H_ */
