#include "core.h"
#include "qt_gui.h"

int main(int argc, char *argv[]) {

  // TODO: temporary workaround to set core UI ...
  sdc::Core* c = sdc::Core::Instance();
  sdc::Core::Instance()->SetUI(new sdc::QtGui(c, argc, argv));

  sdc::Core::Instance()->Start();

  return sdc::Core::Instance()->GetReturnCode();
}
