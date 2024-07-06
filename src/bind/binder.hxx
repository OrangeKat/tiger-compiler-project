/**
 ** \file bind/binder.hxx
 ** \brief Inline methods of bind::Binder.
 **/

// FIXME: Some code was deleted here.
#pragma once

#include <bind/binder.hh>

namespace bind
{
  template <typename T>
  void Binder::undeclared(const std::string& k, const T& e)
  {
    error_ << misc::error::error_type::bind << e.location_get()
           << ": is not declared " << k << ": " << e.name_get() << '\n';
  }

  template <typename T> void Binder::redefinition(const T& e1, const T& e2)
  {
    error_ << misc::error::error_type::bind << e2.name_get()
           << " redefinition at : " << e2.location_get() << std::endl
           << e1.location_get() << " is the first definiton" << '\n';
  }

  template <class D> void Binder::chunk_visit(ast::Chunk<D>& e)
  {
    // Shorthand.
    // using chunk_type = ast::Chunk<D>;
    // DONE: Some code was deleted here (Two passes: once on headers, then on bodies).
    auto& chunk_decs = e.decs_get();
    for (auto i = chunk_decs.begin(); i != chunk_decs.end(); i++)
      {
        auto j = chunk_decs.begin();
        while (j != i && (*j)->name_get().get() != (*i)->name_get().get())
          {
            j++;
          }
        if (j != i)
          {
            redefinition(**j, **i);
          }
        visit_dec_header<D>(**i);
      }
    for (auto& i : e.decs_get())
      visit_dec_body(*i);
  }

  /* These specializations are in bind/binder.hxx, so that derived
     visitors can use them (otherwise, they wouldn't see them).  */

  // DONE: Some code was deleted here.

  template <>
  inline void Binder::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    type_scope_.put(e.name_get(), &e);
  }

  template <>
  inline void Binder::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    function_scope_.put(e.name_get(), &e);
  }

  template <> inline void Binder::visit_dec_header<ast::VarDec>(ast::VarDec& e)
  {
    var_scope_.put(e.name_get(), &e);
  }

  template <> inline void Binder::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    scope_begin();
    e.ty_get().accept(*this);
    scope_end();
  }

  template <>
  inline void Binder::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    scope_begin();
    chunk_visit<ast::VarDec>(e.formals_get());
    if (e.name_get() == "_main")
    {
        main_count++;
    }
    auto result = e.result_get();
    if (result)
      result->accept(*this);
    auto body = e.body_get();
    if (body)
      body->accept(*this);
    scope_end();
  }

  template <> inline void Binder::visit_dec_body<ast::VarDec>(ast::VarDec& e)
  {
      e.accept(*this);
  }
} // namespace bind
