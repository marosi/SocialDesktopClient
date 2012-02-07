/**
 * @file controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 31, 2012
 */

#ifndef B_CONTROLLER_H_
#define B_CONTROLLER_H_

#include "sdc.h"

class BModel;
class BView;

class BController : public QObject, public sdc::ServiceController {

  Q_OBJECT

 public:
  BController();
  void SendDiscoInfo(const std::string &to_attribute, const std::string &node_attribute);
  void SendDiscoItems(const std::string &to_attribute, const std::string &node_attribute);
  // Qt signal wrappers

 signals: // signals in controller are used to manipulate Qt view

 private:
  void ConnectView();
};

#endif /* B_CONTROLLER_H_ */
