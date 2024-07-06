/**
 ** \file misc/scoped-map.hh
 ** \brief Declaration of misc::scoped_map.
 **
 ** This implements a stack of dictionnaries.  Each time a scope is
 ** opened, a new dictionnary is added on the top of the stack; the
 ** dictionary is removed when the scope is closed.  Lookup of keys
 ** is done in the last added dictionnary first (LIFO).
 **
 ** In particular this class is used to implement symbol tables.
 **/

#pragma once

#include <map>
#include <vector>

namespace misc
{
  template <typename Key, typename Data> class scoped_map
  {
    //FIXME: Some code was deleted here.
  public:
    scoped_map();
    ~scoped_map() = default;
    void put(const Key& key, const Data& value);
    Data get(const Key& key) const;
    void scope_begin();
    void scope_end();
    std::ostream& dump(std::ostream& ostr) const;
    std::vector<std::map<Key, Data>> stack;
  };

  template <typename Key, typename Data>
  std::ostream& operator<<(std::ostream& ostr,
                           const scoped_map<Key, Data>& tbl);

  // FIXME: Some code was deleted here.
  template <typename Key, typename Data>
  std::enable_if_t<!std::is_pointer<Data>::value, Data> throw_not_find_error()
  {
    throw(std::range_error("key not in scope"));
  }

  template <typename Key, typename Data>
  std::enable_if_t<std::is_pointer<Data>::value, Data> throw_not_find_error()
  {
    return nullptr;
  }
} // namespace misc

#include <misc/scoped-map.hxx>
