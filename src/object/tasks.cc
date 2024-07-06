/**
 ** \file object/tasks.cc
 ** \brief Object module related tasks' implementation.
 */

#include <ast/tasks.hh>
#include <common.hh>
#include <object/libobject.hh>
#define DEFINE_TASKS 1
#include <object/tasks.hh>
#undef DEFINE_TASKS

namespace object::tasks
{
  void object_parse() {}

  // FIXME: Some code was deleted here.
  //
  void object_bind_comp()
  {
    task_error() << ::object::bind(*ast::tasks::the_program);
    task_error().exit_on_error();
  }

  void object_types_compute()
  {
    task_error() << ::object::types_check(*ast::tasks::the_program)
                 << &misc::error::exit_on_error;
  }
  void object_rename() {}

} // namespace object::tasks
