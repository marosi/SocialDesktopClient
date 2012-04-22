#include "buddycloud_view.h"
#include "contact_frame_view.h"
#include "posts_frame_view.h"
#include <QString>

BuddycloudView::BuddycloudView(BController* controller, QWidget *parent)
    : QWidget(parent),
      controller_(controller)
{
	ui.setupUi(this);
	CreateActions();
	// initiate ui components
	//ui.connectingMessage->setVisible(false);
}

BuddycloudView::~BuddycloudView() {}

void BuddycloudView::CreateActions() {
  connect(ui.onlineState, SIGNAL(currentIndexChanged(const QString &)),
      controller_, SLOT(SwitchOnlineState(const QString &)));
  //
  connect(ui.avatar, SIGNAL(clicked()),
      controller_, SLOT(ShowXmppCommandsWindow()));
}

void BuddycloudView::ShowConnecting() {
  //ui.connectingMessage->setVisible(true);
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
      //ui.connectingMessage->setVisible(false); // hide connecting ...
      return;
    }
  }
  assert(false);
}

void BuddycloudView::ShowContact(const QString &uid, const QString &name) {
/*  ContactFrameView* cframe = new ContactFrameView(ui.contactPanel);
  cframe->SetInfo(name);
  cframe->setEnabled(true);
  cframe->setVisible(true);
  contacts_.append(cframe);
  ui.contactPanelLayout->insertWidget(0, cframe);*/
}

void BuddycloudView::CreatePostsView(QObject* controller) {
  /*PostsFrameView* posts_frame = new PostsFrameView(controller, ui.contentFrame);
  emit signalPostsViewCreated(posts_frame);*/
}
