#include "core.h"

int main(int argc, char *argv[]) {

  sdc::Core core(argc, argv);

  core.Start();

  return core.GetReturnCode();
}
