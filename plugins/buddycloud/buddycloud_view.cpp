#include "buddycloud_view.h"
#include <QString>

BuddycloudView::BuddycloudView(BController* controller, QWidget *parent)
    : QMainWindow(parent),
      controller_(controller)
{
	ui.setupUi(this);
	CreateActions();
	// initiate ui components
	ui.connectingMessage->setVisible(false);
}

BuddycloudView::~BuddycloudView()
{

}

void BuddycloudView::CreateActions() {
  connect(ui.onlineState, SIGNAL(currentIndexChanged(const QString &)),
      controller_, SLOT(SwitchOnlineState(const QString &)));
}

void BuddycloudView::ShowConnecting() {
  ui.connectingMessage->setVisible(true);
}

void BuddycloudView::ShowState(const QString &state) {
  QString upper;
  upper.append(state[0].toUpper());
  upper.append(state.mid(1));
  LOG(DEBUG) << upper.toStdString();
  int item_count = ui.onlineState->count();
  for (int i = 0; i < item_count; ++i) {
    if (ui.onlineState->itemText(i) == upper) {
      ui.onlineState->setCurrentIndex(i);
      ui.connectingMessage->setVisible(false);
      return;
    }
  }
  assert(false);
}
