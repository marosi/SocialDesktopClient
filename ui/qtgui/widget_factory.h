/**
 * @file widget_factory.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 22, 2012
 */

#ifndef WIDGET_FACTORY_H_
#define WIDGET_FACTORY_H_

#include "content.h"
#include <QtGui/QWidget>

namespace sdc {

class WidgetFactory {
 public:
  virtual bool CanCreate(Content::Ref content)=0;
  virtual QWidget* Create(QWidget* parent, Content::Ref content)=0;
};

} /* namespace sdc */

#endif /* WIDGET_FACTORY_H_ */
