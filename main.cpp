#include "core.h"
#include "qt_gui.h"

int main(int argc, char *argv[]) {

  sdc::Core::Create(new sdc::QtGui(argc, argv));

  sdc::Core::Instance()->Start();

  return sdc::Core::Instance()->GetReturnCode();
}
