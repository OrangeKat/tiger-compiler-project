/**
 ** \file bind/tasks.hh
 ** \brief Bind module related tasks.
 */

// FIXME: Some code was deleted here.
#pragma once

#include "task/libtask.hh"

namespace bind::tasks
{
  TASK_GROUP("5. The Bind Group");

  TASK_DECLARE("B|bindings-display", "display binds", bind_disp, "");

  TASK_DECLARE("b|bindings-compute", "compute binds", bind_comp, "parse");

  TASK_DECLARE("bound", "idk but something bound", bound, "bindings-compute");

  TASK_DECLARE("rename",
               "rename stuff something bind too idk tbh",
               rename,
               "bindings-compute");
} // namespace bind::tasks
