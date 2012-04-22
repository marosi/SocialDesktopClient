/**
 * @file generic_widget_factory.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 22, 2012
 */

#ifndef GENERIC_WIDGET_FACTORY_H_
#define GENERIC_WIDGET_FACTORY_H_

#include "content.h"
#include "widget_factory.h"

namespace sdc {

template<class C, class W>
class GenericWidgetFactory : public WidgetFactory {
 public:
  virtual bool CanCreate(Content::Ref content) {
    return boost::dynamic_pointer_cast<C>(content);
  }

  virtual QWidget* Create(QWidget* parent, Content::Ref content) {
    boost::shared_ptr<C> casted = boost::dynamic_pointer_cast<C>(content);
    return new W(parent, casted);
  }
};

} /* namespace sdc */

#endif /* GENERIC_WIDGET_FACTORY_H_ */
