/**
 ** \file ast/escapable.hxx
 ** \brief Inline methods of ast::Escapable.
 */

#pragma once

#include <ast/escapable.hh>

namespace ast
{
  inline bool Escapable::get_escapable() const { return escape_; }
  inline bool Escapable::get_escapable() { return escape_; }
  inline void Escapable::set_escapable(bool escape) { this->escape_ = escape; }
  // FIXME: Some code was deleted here.
} // namespace ast
