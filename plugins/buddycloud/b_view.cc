
#include "b_view.h"
#include "b_controller.h"
#include "xmpp_commands.h"
#include <QString>

BView::BView(QWidget* parent) {
  //window_ = new QMainWindow;
  //window_->show();
  xmpp_ = new XmppCommands;
  xmpp_->setWindowTitle("XMPP Commands");
  xmpp_->show();
  CreateActions();
}

void BView::DoUpdate() {
}

void BView::DiscoInfoClicked() {
  QString to = xmpp_->ui.discoInfoToAttribute->text();
  QString node = xmpp_->ui.discoInfoNodeAttribute->text();
  GetController<BController>()->SendDiscoInfo(to.toUtf8().constData(), node.toUtf8().constData()  );
}

void BView::DiscoItemsClicked() {
  QString to = xmpp_->ui.discoItemsToAttribute->text();
  QString node = xmpp_->ui.discoItemsNodeAttribute->text();
  GetController<BController>()->SendDiscoItems(to.toUtf8().constData(), node.toUtf8().constData());
}

void BView::ShowMessage(const QString &string) {
  LOG(DEBUG3) << "SHOWWWWWWWWWWWWING MESSSSSSAAAGE";
}

void BView::CreateActions() {
  QObject::connect(xmpp_->ui.discoInfo, SIGNAL(clicked()), this,
      SLOT(DiscoInfoClicked()));
  QObject::connect(xmpp_->ui.discoItems, SIGNAL(clicked()), this,
      SLOT(DiscoItemsClicked()));
}
