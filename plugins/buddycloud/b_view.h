/**
 * @file view.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 31, 2012
 */

#ifndef B_VIEW_H_
#define B_VIEW_H_

#include "sdc.h"

class BController;
class BModel;
class XmppCommands;

class BView : public QWidget, public sdc::ServiceView {

  Q_OBJECT

 public:

  BView(QWidget* parent = 0);

 private slots:
  void DoUpdate();
  void DiscoInfoClicked();
  void DiscoItemsClicked();
  void SomeButtonClicked();
  void ShowMessage(const QString &string = "DEFAULT MESSAGE");

 private:
  void CreateActions();

  XmppCommands* xmpp_;
  QMainWindow* window_;
};

#endif /* B_VIEW_H_ */
