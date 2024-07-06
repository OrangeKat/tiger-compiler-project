/**
 ** \file bind/renamer.cc
 ** \brief Implementation of bind::Renamer.
 */

#include <bind/renamer.hh>

namespace bind
{
  using namespace ast;

  // FIXME: Some code was deleted here.
  // ADD MAP TO TRACK OLD NAMES AND NEW NAMES - done
  Renamer::Renamer()
    : super_type()
    , names_()
  {}

  void Renamer::operator()(FunctionDec& e)
  {
    if (names_.find(&e) == names_.end() && e.name_get() != "_main"
        && e.body_get() != nullptr)
      {
        super_type::operator()(e);
        e.name_set(e.name_get().fresh(e.name_get()));
        names_[&e] = e.name_get();
      }
    else
      {
        super_type::operator()(e);
      }
  }

  void Renamer::operator()(TypeDec& e)
  {
    if (names_.find(&e) == names_.end())
      {
        e.name_set(e.name_get().fresh(e.name_get()));
        names_[&e] = e.name_get();
      }
    super_type::operator()(e);
  }

  void Renamer::operator()(VarDec& e)
  {
    if (names_.find(&e) == names_.end())
      {
        e.name_set(e.name_get().fresh(e.name_get()));
        names_[&e] = e.name_get();
      }
    super_type::operator()(e);
  }

  void Renamer::operator()(CallExp& e)
  {
    if (e.def_get() == nullptr)
      {
        return;
      }

    if (names_.find(e.def_get()) == names_.end())
      {
        super_type::operator()(e.def_get());
      }

    e.name_set(names_[e.def_get()]);
    super_type::operator()(e);
  }

  void Renamer::operator()(NameTy& e)
  {
    if (e.def_get() == nullptr)
      {
        return;
      }

    if (names_.find(e.def_get()) == names_.end())
      {
        super_type::operator()(e.def_get());
      }

    e.name_set(names_[e.def_get()]);
    super_type::operator()(e);
  }

  void Renamer::operator()(SimpleVar& e)
  {
    if (e.def_get() == nullptr)
      {
        return;
      }

    if (names_.find(e.def_get()) == names_.end())
      {
        super_type::operator()(e.def_get());
      }

    e.name_set(names_[e.def_get()]);
    super_type::operator()(e);
  }

} // namespace bind
