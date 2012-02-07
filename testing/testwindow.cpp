#include "testwindow.h"

#include "test_model.h"
#include "test_controller.h"
/*
// TODO: move this code elswhere
#include "QtGui/QKeyEvent"
#include "QtGui/QTextEdit"
#include "simple_message.h"
#include "message.h"

class EnterKeyInterceptor : QObject {

    QObject

 public:
  EnterKeyInterceptor(QTextEdit* text_edit) {
    text_edit_ = text_edit;
  }
  bool eventFilter(QObject* obj, QEvent* event) {
    if(event->type() == QEvent::KeyPress) {
      QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
      if(key_event->key() == Qt::Key_Enter) {

      }
    }
    return false;
  }
 private:

  QTextEdit* text_edit_;
};*/

#include "boost/pointer_cast.hpp"

TestWindow::TestWindow(QWidget* parent) :
    QMainWindow(parent)
{
  ui_.setupUi(this);
  CreateActions();
  /*
   EnterKeyInterceptor* filter = new EnterKeyInterceptor();
   ui.textEdit->installEventFilter(filter);
   */
}

TestWindow::~TestWindow() {}

void TestWindow::Update() {
  emit update();
}

std::string TestWindow::GetText() {
  return ui_.textEdit->toPlainText().toUtf8().constData();
}

void TestWindow::ClearText() {
  ui_.textEdit->setText("");
}

void TestWindow::OnConfirmText() {
  GetController()->SendXmppStanza();
}

void TestWindow::DoUpdate() {
  ui_.textBrowser->append(QString::fromStdString(GetModel()->GetLastlyAddedContent()));
}

void TestWindow::CreateActions() {
  QObject::connect(ui_.textEdit, SIGNAL(onEnter()), this, SLOT(OnConfirmText()));
  QObject::connect(this, SIGNAL(update()), this, SLOT(DoUpdate()));
}
