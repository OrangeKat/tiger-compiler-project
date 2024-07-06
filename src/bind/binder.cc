/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <ast/all.hh>
#include <bind/binder.hh>

#include <misc/contract.hh>

namespace bind
{
  int Binder::main_count = 0;
  bool Binder::visiting_chunk = false;

  Binder::Binder() { scope_begin(); };
  Binder::~Binder() { scope_end(); }

  void Binder::scope_begin()
  {
    type_scope_.scope_begin();
    var_scope_.scope_begin();
    function_scope_.scope_begin();
  }
  void Binder::scope_end()
  {
    type_scope_.scope_end();
    var_scope_.scope_end();
    function_scope_.scope_end();
  }

  void Binder::operator()(ast::LetExp& e)
  {
    // DONE: Some code was deleted here.
    scope_begin();

    e.chunks_get().accept(*this);
    e.body_get().accept(*this);

    scope_end();
  }

  void Binder::operator()(ast::SimpleVar& e)
  {
    auto* res = var_scope_.get(e.name_get());
    if (res)
      {
        e.def_set(res);
      }
    else
      {
        undeclared("variable", e);
      }
  }

  void Binder::operator()(ast::ForExp& e)
  {
    scope_begin();

    var_scope_.put(e.vardec_get().name_get(), &(e.vardec_get()));
    e.vardec_get().accept(*this);
    e.hi_get().accept(*this);

    loop_scope_.push_back(&e);
    e.body_get().accept(*this);
    loop_scope_.pop_back();

    scope_end();
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    scope_begin();

    visiting_chunk = true;
    e.test_get().accept(*this);
    visiting_chunk = false;

    loop_scope_.push_back(&e);
    e.body_get().accept(*this);
    loop_scope_.pop_back();

    scope_end();
  }

  void Binder::operator()(ast::BreakExp& e)
  {
    if (visiting_chunk || loop_scope_.size() == 0)
      {
        break_outside_loop(e);
      }
    else
      {
        auto res = loop_scope_.back();
        e.loop_set(res);
      }
  }

  void Binder::operator()(ast::CallExp& e)
  {
    auto def = function_scope_.get(e.name_get());
    if (def)
      {
        e.def_set(def);
        for (auto& i : e.args_get())
          i->accept(*this);
      }
    else
      {
        undeclared("function", e);
      }
  }
  void Binder::operator()(ast::ChunkList& e)
  {
    for (auto& i : e.chunks_get())
      i->accept(*this);
  }

  void Binder::operator()(ast::NameTy& e)
  {
    auto def = type_scope_.get(e.name_get());
    if (e.name_get().get() == "string" || e.name_get().get() == "int")
      {
        e.def_set(nullptr);
      }
    else if (def)
      {
        e.def_set(def);
      }
    else
      {
        undeclared("type", e);
      }
  }

  void Binder::operator()(ast::VarChunk& e)
  {
    visiting_chunk = true;
    chunk_visit<ast::VarDec>(e);
    visiting_chunk = false;
  }

  void Binder::operator()(ast::VarDec& e)
  {
      scope_begin();
      if (e.type_name_get())
          e.type_name_get()->accept(*this);
      if (e.init_get())
          e.init_get()->accept(*this);
      scope_end();
  }
  void Binder::operator()(ast::FunctionChunk& e)
  {
    visiting_chunk = true;
    chunk_visit<ast::FunctionDec>(e);
    visiting_chunk = false;
  }

  void Binder::operator()(ast::TypeChunk& e) { chunk_visit<ast::TypeDec>(e); }
  // DONE: Some code was deleted here.
  /*-----------------.
      | Error handling.  |
      `-----------------*/

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  // DONE? Some code was deleted here.

  void Binder::break_outside_loop(ast::BreakExp& e)
  {
    error_ << misc::error::error_type::bind << e.location_get()
           << ": break is out of scope" << '\n';
  }

  void Binder::check_main(const ast::FunctionDec& e)
  {
    if (e.name_get() == "_main")
      {
        main_count++;
        if (main_count > 1)
          {
            error_ << misc::error::error_type::bind << e.location_get()
                   << ": multiple main functions" << std::endl;
          }
      }
  }

} // namespace bind
