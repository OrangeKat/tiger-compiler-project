/**
 ** \file escapes/escapes-visitor.cc
 ** \brief Implementation for escapes/escapes-visitor.hh.
 */

#include <ast/all.hh>
#include <escapes/escapes-visitor.hh>
#include <misc/contract.hh>

namespace escapes
{
  // FIXME: Some code was deleted here.

  void EscapesVisitor::operator()(ast::VarDec& e)
  {
    map_.put(&e, curr);
    e.set_escapable(false);
    if (e.type_name_get())
      e.type_name_get()->accept(*this);
    if (e.init_get())
      e.init_get()->accept(*this);
  }
  void EscapesVisitor::operator()(ast::FunctionDec& e)
  {
    auto old = curr;
    curr = &e;
    scope_begin();
    chunk_visit<ast::VarChunk>(e.formals_get());
    if (e.result_get())
      e.result_get()->accept(*this);
    if (e.body_get())
      e.body_get()->accept(*this);
    scope_end();
    curr = old;
  }
  void EscapesVisitor::operator()(ast::SimpleVar& e)
  {
    if (map_.get(e.def_get()) != nullptr && map_.get(e.def_get()) != curr)
      {
        e.def_get()->set_escapable(true);
      }
  }
  void EscapesVisitor::scope_begin() { this->map_.scope_begin(); }
  void EscapesVisitor::scope_end() { this->map_.scope_end(); }
} // namespace escapes
