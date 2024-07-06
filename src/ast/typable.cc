/**
 ** \file ast/typable.cc
 ** \brief Implementation of ast::Typable.
 */

#include <ast/typable.hh>
#include <ast/visitor.hh>

namespace ast
{
  // FIXME: Some code was deleted here.
  Typable::Typable(const type::Type* type)
    : type_(type)
  {}
} // namespace ast
