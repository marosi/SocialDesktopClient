/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "core.h"
#include "qt_gui.h"
#include "boost/program_options.hpp"
#include <string>

int main(int argc, char* argv[]) {

  namespace po = boost::program_options;

  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("homedir", po::value<std::string>(), "set home directory")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("homedir")) {
    chdir(vm["homedir"].as<std::string>().c_str());
  }

  sdc::Core::Create(new sdc::QtGui(argc, argv));

  sdc::Core::Instance()->Start();

  return sdc::Core::Instance()->GetReturnCode();
}
