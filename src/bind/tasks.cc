/**
 ** \file bind/tasks.cc
 ** \brief Bind module tasks implementation.
 */

// FIXME: Some code was deleted here.
#include <ast/libast.hh>
#include <ast/tasks.hh>
#include <bind/libbind.hh>
#include <common.hh>
#include <parse/libparse.hh>

#define DEFINE_TASKS 4
#include <bind/tasks.hh>
#undef DEFINE_TASKS

namespace bind::tasks
{
  void rename() { bind::rename(*ast::tasks::the_program); }

  void bind_disp() { ast::bindings_display(std::cout) = true; }

  void bind_comp()
  {
    task_error() << ::bind::bind(*ast::tasks::the_program);
    task_error().exit_on_error();
  }

  void bound() {}
} // namespace bind::tasks
