/**
 ** \file ast/escapable.hh
 ** \brief Declaration of ast::Escapable.
 */

#pragma once

#include <ast/fwd.hh>

namespace ast
{
  /// Escapable.
  class Escapable
  {
  public:
    Escapable();
    virtual ~Escapable() = default;
    bool get_escapable();
    bool get_escapable() const;
    void set_escapable(bool);

  private:
    bool escape_;
    // FIXME: Some code was deleted here.
  };
} // namespace ast
#include <ast/escapable.hxx>
