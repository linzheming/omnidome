/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOOSTX_FACTORY_HPP_
#define BOOSTX_FACTORY_HPP_

#include <unordered_map>
#include <functional>
#include <type_traits>
#include <omni/global.h>

namespace boostx
{
  /**@brief The central factory class.
   * @tparam INTERFACE Typename of the abstract interface
   * @tparam TYPEINFO Template template parameter for the typeinfo
   * @tparam ...ARGS Template parameter which comply to the constructor signature of the interface
   */
  template<typename INTERFACE, template<class> class TYPEINFO, typename...ARGS>
  struct factory
  {
    /// Typedef for our abstract interface
    typedef INTERFACE interface_type;

    /// Typedef for this factory type
    typedef factory<interface_type,TYPEINFO,ARGS...> type;

    /// Template alias for typeinfo type
    template<typename T>
    using typeinfo_type = TYPEINFO<T>;

    /// Get key type from typeinfo struct
    typedef typename typeinfo_type<interface_type>::typeid_type key_type;

    /// Constructor function type
    typedef std::function<interface_type*(ARGS...)> constructor_type;

    /// Internal type of map for storing ids
    typedef std::unordered_map<key_type,constructor_type> class_map_type;

    /// Checks if class with element id is registered
    bool exists(const key_type& _key) const
    {
      return _classes.count(_key) > 0;
    }

    /// Registers class with custom key
    template<typename T>
    void reg(const key_type& _key)
    {
      if (exists(_key)) return;
      _classes[_key] = [](ARGS&&...args){ return new T(args...); };
    }

    /// Registers class by getting key from registrar's type id
    template<typename T>
    void reg()
    {
      key_type_check<T>();
      reg<T>(type_id<T>());
    }

    /// Unregisters class with custom key
    void unreg(const key_type& _key)
    {
      if (!exists(_key)) return;
      _classes.erase(_key);
    }

    /// Unregisters class by getting key from registrar
    template<typename T>
    void unreg()
    {
      key_type_check<T>();
      unreg(type_id<T>());
    }

    /// Instantiates an object of the class by id and constructor arguments
    interface_type* create(const key_type& _key, const ARGS&..._args) const
    {
      if (!exists(_key)) return nullptr;
      return _classes.at(_key)(_args...);
    }

    /// Gives readonly access to registered classes
    class_map_type const& classes()
    {
      return _classes;
    }



  private:
    /// Checks if key types are the same
    template<typename T>
    void key_type_check()
    {
      // Check if T is a base class of interface
      static_assert(std::is_base_of<interface_type,T>::value,
          "Given type T must be a base class of the interface class");

      // Check if registrar's type id and key type are the same
      typedef std::is_same<decltype(type_id<T>()),key_type> is_same_type;
      static_assert(is_same_type::value,
          "Type of type id to be registered and registrys type id must be equal!");
    }

    template<typename T>
    /// Return type id by calling the call operator of type info template
    decltype(typeinfo_type<T>()()) type_id() const
    {
      return typeinfo_type<T>()();
    }

    class_map_type _classes;

  };
}


#endif /* BOOSTX_FACTORY_HPP_ */
