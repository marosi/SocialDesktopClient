/**
 * @file qt_service.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 14, 2012
 */

#ifndef QT_SERVICE_H_
#define QT_SERVICE_H_

namespace sdc {

class NewAccountWidget;

class QtService : public Service {
 public:
  virtual NewAccountWidget* CreateNewAccountPane()=0;
};

} /* namespace sdc */

#endif /* QT_SERVICE_H_ */
