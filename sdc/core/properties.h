/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_PROPERTIES_H_
#define CORE_PROPERTIES_H_

#include "boost/serialization/nvp.hpp"
#include "boost/serialization/map.hpp"

namespace sdc {

/**
 * Properties class provides general interface for working with various properties,
 * which require to outlive application runtime.
 *
 * It can be used either alone as a class member or by inheriting from it.
 */
class Properties {
 public:
  friend class boost::serialization::access;

  /**
   * Set particular property.
   * @param key key identifying property
   * @param value property value
   */
  void SetProperty(const std::string &key, const std::string &value) {
    properties_[key] = value;
  }

  /**
   * Get property by its key.
   * @param key property key
   * @return property in form of a string
   */
  std::string GetProperty(const std::string &key) {
    if (properties_.count(key) > 0)
      return properties_[key];
    else
      return std::string();
  }

 private:
  std::map<std::string, std::string> properties_;

  /**
   * Boost serialization method
   */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    if (version) {}
    ar & BOOST_SERIALIZATION_NVP(properties_);
  }
};

} // namespace sdc

#endif // CORE_PROPERTIES_H_
