/**
 ** \file ast/type-constructor.hxx
 ** \brief Inline methods of ast::TypeConstructor.
 */

#pragma once

#include <ast/type-constructor.hh>
#include <type/types.hh>

namespace ast
{
  // FIXME: Some code was deleted here.
  inline const type::Type* TypeConstructor::new_type_get() const
  {
    return new_type_;
  }

  inline void TypeConstructor::new_type_set(const type::Type* new_type)
  {
    new_type_ = new_type;
  }
} // namespace ast
