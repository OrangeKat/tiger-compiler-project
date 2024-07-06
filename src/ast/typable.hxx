/**
 ** \file ast/typable.hxx
 ** \brief Inline methods of ast::Typable.
 */

#pragma once

#include <ast/typable.hh>

namespace ast
{
  // FIXME: Some code was deleted here.
  inline const type::Type* Typable::get_type() const { return type_; }

  inline void Typable::type_set(const type::Type* type) { type_ = type; }
} // namespace ast
