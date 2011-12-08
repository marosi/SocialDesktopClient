#include "mainwindow.h"

#include <QtGui>
#include <QApplication>

// Used for testing purposes
#include "testing/testconsole.ui.h"
#include "testing/testwindow.h"

#include "core.h"

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  TestWindow w;
  sdc::Core core;

  core.SetGui(&w);
  core.Start();

  //sdc::UI ui(&core);
  //ui.Run();


  w.show();
  int gui_result = a.exec();

  /*
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
  */

  LOG(INFO) << "Exiting application...";
  core.Exit();
  core.Join();

  return gui_result;
}
