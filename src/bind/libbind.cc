/**
 ** \file bind/libbind.cc
 ** \brief Define exported bind functions.
 */
#include <bind/binder.hh>
#include <bind/libbind.hh>
#include <bind/renamer.hh>

namespace bind
{
  misc::error bind(ast::Ast& ast)
  {
    Binder binder;
    binder(ast);
    return binder.error_get();
  }
  void rename(ast::Ast& ast)
  {
    Renamer rename;
    rename(ast);
  }
} // namespace bind
// FIXME: Some code was deleted here.
