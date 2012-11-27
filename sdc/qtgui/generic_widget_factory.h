/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef GENERIC_WIDGET_FACTORY_H_
#define GENERIC_WIDGET_FACTORY_H_

#include "widget_factory.h"

namespace sdc {

//template<class C, class W>
//class GenericWidgetFactory : public WidgetFactory {
// public:
//  virtual bool CanCreate(Content::Ref content) {
//    return boost::dynamic_pointer_cast<C>(content);
//  }

//  virtual QWidget* Create(QWidget* parent, Content::Ref content) {
//    boost::shared_ptr<C> casted = boost::dynamic_pointer_cast<C>(content);
//    return new W(parent, casted);
//  }
//};

} // namespace sdc

#endif // GENERIC_WIDGET_FACTORY_H_
