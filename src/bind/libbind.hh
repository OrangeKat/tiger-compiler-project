/**
 ** \file bind/libbind.hh
 ** \brief Interface of the bind module.
 */

#include <misc/error.hh>

namespace bind
{
  misc::error bind(ast::Ast& ast);
  void rename(ast::Ast& ast);
} // namespace bind

// FIXME: Some code was deleted here.
