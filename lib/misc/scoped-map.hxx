/** \file misc/scoped-map.hxx
 ** \brief Implementation of misc::scoped_map.
 */

#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <ranges>
#include <misc/algorithm.hh>
#include <misc/contract.hh>
#include <misc/indent.hh>
#include "scoped-map.hh"
namespace misc
{
  // FIXME: Some code was deleted here.
  template <typename Key, typename Data> scoped_map<Key, Data>::scoped_map() {}

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::put(const Key& key, const Data& value)
  {
    stack.back()[key] = value;
  }
  template <typename Key, typename Data>
  Data scoped_map<Key, Data>::get(const Key& key) const
  {
    for (size_t i = stack.size(); i > 0; i--)
      {
        auto map = stack.at(i - 1);
        if (map.contains(key))
          {
            return map.find(key)->second;
          }
      }
    return throw_not_find_error<Key, Data>();
  }
  template <typename Key, typename Data>
  std::ostream& scoped_map<Key, Data>::dump(std::ostream& ostr) const
  {
    for (auto map : stack)
      {
        ostr << "{";
        for (auto e : map)
          {
            ostr << "( " << e->first << "=" << e->second << " )";
          }
        ostr << "}\n";
      }
    return ostr;
  }
  template <typename Key, typename Data>
  void scoped_map<Key, Data>::scope_begin()
  {
    stack.push_back(std::map<Key, Data>());
  }
  template <typename Key, typename Data> void scoped_map<Key, Data>::scope_end()
  {
    stack.pop_back();
  };

  template <typename Key, typename Data>
  inline std::ostream& operator<<(std::ostream& ostr,
                                  const scoped_map<Key, Data>& tbl)
  {
    return tbl.dump(ostr);
  }

} // namespace misc
