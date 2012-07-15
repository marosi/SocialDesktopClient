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
#include <QList>
#include <string>

class PostWidget;

class ChannelWidget : public sdc::ContentPanel, public AbstractPresenter {

    Q_OBJECT

 public:
  ChannelWidget(AbstractPresenter* presenter, ChannelController* channel);

 public slots:
  void SendPost();
  void OnScrollBarValueChanged(int value);

 private:
  ChannelController* channel_;
  QMap<std::string, PostWidget*> posts_;
  QToolButton* new_post_button_;
  QWidget* new_post_;
  Ui::NewPostClass new_post_ui;
  QScrollBar* scroll_bar_;
  int old_scroll_bar_value_;
};

#endif /* CHANNEL_WIDGET_H_ */
