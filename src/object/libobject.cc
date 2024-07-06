/**
 ** \file object/libobject.cc
 ** \brief Define exported object functions.
 */

// FIXME: Some code was deleted here.
#include <bind/binder.hh>
#include <object/binder.hh>
#include <object/libobject.hh>
#include <object/type-checker.hh>

namespace object
{
  /*-------.
  | Bind.  |
  `-------*/
  misc::error bind(ast::Ast& e)
  {
    Binder binder;
    binder(e);
    return binder.error_get();
  }
  // FIXME: Some code was deleted here.
  misc::error bind_obj(ast::Ast& e)
  {
    bind::Binder binder;
    binder(e);
    return binder.error_get();
  }

  void rename(ast::Ast& ast) {}

  /*----------------.
  | Compute types.  |
  `----------------*/

  misc::error types_check(ast::Ast& tree)
  {
    TypeChecker type;
    type(tree);
    return type.error_get();
  }

} // namespace object
