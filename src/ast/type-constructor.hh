/**
 ** \file ast/type-constructor.hh
 ** \brief Declaration of ast::TypeConstructor.
 */

#pragma once

#include <ast/fwd.hh>
#include <type/fwd.hh>

namespace ast
{
  /** \class ast::TypeConstructor
   ** \brief Create a new type.
   */

  class TypeConstructor
  {
    // FIXME: Some code was deleted here.
  public:
    TypeConstructor();
    TypeConstructor(const type::Type* e);
    virtual ~TypeConstructor();

    const type::Type* new_type_get() const;
    void new_type_set(const type::Type*);

    virtual void accept(ConstVisitor& v) const = 0;
    virtual void accept(Visitor& v) = 0;

  private:
    const type::Type* new_type_ = nullptr;
  };
} // namespace ast
#include <ast/type-constructor.hxx>
