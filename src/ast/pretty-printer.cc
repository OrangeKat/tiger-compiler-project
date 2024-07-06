/**
 ** \file ast/pretty-printer.cc
 ** \brief Implementation of ast::PrettyPrinter.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <ast/pretty-printer.hh>
#include <misc/escape.hh>
#include <misc/indent.hh>
#include <misc/separator.hh>

#include <type/class.hh>

namespace ast
{
  // Anonymous namespace: these functions are private to this file.
  namespace
  {

    /// \brief Output \a e on \a ostr.
    ///
    /// Used to factor the output of the name declared,
    /// and its possible additional attributes.

    inline std::ostream& operator<<(std::ostream& ostr, const Escapable& e)
    {
      if (escapes_display(ostr) && e.get_escapable()
          // FIXME: Some code was deleted here.
      )
        ostr << "/* escaping */ ";
      return ostr;
    }
    inline std::ostream& operator<<(std::ostream& ostr, const Dec& e)
    {
      ostr << e.name_get();
      if (bindings_display(ostr))
        ostr << " /* " << &e << " */";
      return ostr;
    }

  } // namespace

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_(ostr)
  {}

  void PrettyPrinter::operator()(const SimpleVar& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
  }

  void PrettyPrinter::operator()(const FieldVar& e)
  {
    // FIXME: Some code was deleted here.
    ostr_ << e.var_get() << "." << e.name_get();
  }

  /* Foo[10]. */
  void PrettyPrinter::operator()(const SubscriptVar& e)
  {
    ostr_ << e.var_get() << '[' << misc::incindent << e.index_get()
          << misc::decindent << ']';
  }

  void PrettyPrinter::operator()(const CastExp& e)
  {
    ostr_ << "_cast(" << e.exp_get() << ", " << e.ty_get() << ')';
  }

  // FIXME: Some code was deleted here.
  void PrettyPrinter::operator()(const ArrayExp& e)
  {
    ostr_ << e.type_name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e.type_name_get() << " */";
    ostr_ << "[" << e.size_get() << "] of " << e.init_get();
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << "array of " << e.base_type_get();
  }

  void PrettyPrinter::operator()(const AssignExp& e)
  {
    ostr_ << e.var_get() << " := " << e.exp_get();
  }

  void PrettyPrinter::operator()(const BreakExp& e)
  {
    ostr_ << "break";
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.loop_get() << " */";
  }

  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
    ostr_ << '(' << misc::incindent;
    if (e.args_get().size() == 0)
      {
        ostr_ << misc::decindent << ')';
        if (bindings_display(ostr_))
          ostr_ << " /* " << e.def_get() << " */";
        return;
      }
    for (auto var : e.args_get())
      {
        if (var == e.args_get().at(e.args_get().size() - 1))
          {
            ostr_ << *var;
            continue;
          }
        ostr_ << *var << ",";
      }
    ostr_ << misc::decindent << ")";
  }

  void PrettyPrinter::operator()(const ClassTy& e)
  {
    if (&e.super_get() == nullptr)
      ostr_ << "class extends Object";
    else
      ostr_ << "class extends " << e.super_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e.super_get() << " */";
    ostr_ << misc::iendl << "{" << misc::incindent;
    for (auto iter = --e.chunks_get().chunks_get().end();
         iter != e.chunks_get().chunks_get().begin(); iter--)
      {
        ostr_ << misc::iendl << **iter;
      }
    if (e.chunks_get().chunks_get().size() != 0)
      ostr_ << misc::iendl << **e.chunks_get().chunks_get().begin();
    ostr_ << misc::decendl << "}" << misc::iendl;
  }

  void PrettyPrinter::operator()(const ChunkList& e)
  {
    for (auto var : e.chunks_get())
      {
        this->operator()(var);
        if (var != e.chunks_get().back())
          ostr_ << misc::iendl;
      }
  }

  void PrettyPrinter::operator()(const FieldInit& e)
  {
    ostr_ << e.name_get() << " = " << e.init_get();
  }

  void PrettyPrinter::operator()(const Field& e)
  {
    ostr_ << e.name_get() << " : " << e.type_name_get();
  }

  void PrettyPrinter::operator()(const ForExp& e)
  {
    ostr_ << "for ";
    if (bindings_display(ostr_))
      ostr_ << "/* " << &e << " */ ";
    ostr_ << e.vardec_get().name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e.vardec_get() << " */";
    ostr_ << " := " << *e.vardec_get().init_get() << " to " << e.hi_get()
          << " do " << misc::incendl << e.body_get() << misc::decendl;
  }

  void PrettyPrinter::operator()(const FunctionChunk& e)
  {
    size_t i = 0;
    for (auto* a : e.decs_get())
      {
        if (i++ != 0)
          ostr_ << misc::iendl;
        this->operator()(*a);
      }
  }

  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    if (e.body_get() == nullptr)
      {
        ostr_ << "primitive " << e.name_get();
        if (bindings_display(ostr_))
          ostr_ << " /* " << &e << " */";
        ostr_ << '(' << misc::incindent << e.formals_get() << misc::decindent;
        ostr_ << ")";
        if (e.result_get() != nullptr)
          {
            ostr_ << " : " << e.result_get()->name_get();
            if (bindings_display(ostr_))
              ostr_ << " /* " << e.result_get()->def_get() << " */";
          }
      }
    else
      {
        ostr_ << "function " << e.name_get();
        if (bindings_display(ostr_))
          ostr_ << " /* " << &e << " */";
        ostr_ << '(' << misc::incindent << e.formals_get() << misc::decindent;
        ostr_ << ")";
        if (e.result_get() != nullptr)
          {
            ostr_ << " : " << e.result_get()->name_get();
            if (bindings_display(ostr_))
              ostr_ << " /* " << e.result_get()->def_get() << " */";
          }
        if (e.body_get() != nullptr)
          {
            ostr_ << " =" << misc::incendl << *e.body_get() << misc::decindent;
          }
      }
  }

  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "if " << e.test_get() << misc::incendl << " then "
          << e.thenclause_get() << misc::iendl;
    ostr_ << " else " << e.elseclause_get() << misc::decindent;
  }

  void PrettyPrinter::operator()(const IntExp& e) { ostr_ << e.value_get(); }

  void PrettyPrinter::operator()(const LetExp& e)
  {
    ostr_ << "let " << misc::incendl << e.chunks_get() << misc::decendl << "in "
          << misc::incendl << e.body_get() << misc::decendl << "end";
  }

  void PrettyPrinter::operator()(const MethodCallExp& e)
  {
    ostr_ << e.object_get() << '.' << e.name_get() << '(' << misc::incindent;
    if (e.args_get().size() == 0)
      {
        ostr_ << misc::decindent << ')';
        return;
      }

    int i = 0;
    for (; i < e.args_get().size() - 1; i++)
      {
        ostr_ << *e.args_get().at(i) << ",";
      }
    if (e.args_get().at(i))
        ostr_ << *e.args_get().at(i) << misc::decindent << ')';
  }

  void PrettyPrinter::operator()(const MethodChunk& e)
  {
    size_t i = 0;
    for (auto* a : e.decs_get())
      {
        if (i++ != 0)
          ostr_ << misc::iendl;
        this->operator()(*a);
      }
  }

  void PrettyPrinter::operator()(const MethodDec& e)
  {
    ostr_ << "method " << e.name_get() << '(' << misc::incindent
          << e.formals_get() << misc::decindent << ")";
    if (e.result_get() != nullptr)
      ostr_ << " : " << e.result_get()->name_get();
    ostr_ << " = " << *e.body_get();
  }

  void PrettyPrinter::operator()(const NameTy& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
  }

  void PrettyPrinter::operator()(const NilExp& e) { ostr_ << "nil"; }

  void PrettyPrinter::operator()(const ObjectExp& e)
  {
    ostr_ << "new ";
    ostr_ << e.type_name_get();
  }

  void PrettyPrinter::operator()(const OpExp& e)
  {
    ostr_ << "(" << e.left_get() << ' ';
    auto op = e.oper_get();
    switch (op)
      {
      case ast::OpExp::Oper::add:
        ostr_ << "+";
        break;
      case ast::OpExp::Oper::sub:
        ostr_ << "-";
        break;
      case ast::OpExp::Oper::mul:
        ostr_ << "*";
        break;
      case ast::OpExp::Oper::div:
        ostr_ << "/";
        break;
      case ast::OpExp::Oper::eq:
        ostr_ << "=";
        break;
      case ast::OpExp::Oper::ne:
        ostr_ << "<>";
        break;
      case ast::OpExp::Oper::lt:
        ostr_ << "<";
        break;
      case ast::OpExp::Oper::le:
        ostr_ << "<=";
        break;
      case ast::OpExp::Oper::gt:
        ostr_ << ">";
        break;
      case ast::OpExp::Oper::ge:
        ostr_ << ">=";
        break;
      default:
        break;
      }
    ostr_ << " " << e.right_get() << ")";
  }

  void PrettyPrinter::operator()(const RecordExp& e)
  {
    ostr_ << e.type_name_get() << " { ";
    if (e.fields_get().size() == 0)
      {
        ostr_ << '}';
        return;
      }
    int i = e.fields_get().size() - 1;
    for (; i > 0; i--)
      {
        ostr_ << *e.fields_get().at(i) << ", ";
      }
    ostr_ << *e.fields_get().at(i);
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const RecordTy& e)
  {
    ostr_ << "{ ";
    for (auto& var : e.fields_get())
      {
        if (var != e.fields_get().back())
          ostr_ << *var << ", ";
        else
          ostr_ << *var;
      }
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const SeqExp& e)
  {
    ostr_ << "(";
    if (e.exps_get().size() == 0)
      {
        ostr_ << ')';
        return;
      }
    int i = e.exps_get().size() - 1;
    if (i == 0)
      {
        if (e.exps_get().at(0))
          ostr_ << *e.exps_get().at(0);
        ostr_ << ")";
        return;
      }
    ostr_ << misc::incendl;
    for (; i >= 0; i--)
      {
        if (!e.exps_get().at(i))
          continue;
        ostr_ << *e.exps_get().at(i);
        if (i != 0)
          {
            ostr_ << ";";
            ostr_ << misc::iendl;
          }
      }
    ostr_ << misc::decendl << ")";
  }

  void PrettyPrinter::operator()(const StringExp& e)
  {
    ostr_ << '"' << misc::escape(e.value_get()) << '"';
  }

  void PrettyPrinter::operator()(const TypeChunk& e)
  {
    size_t i = 0;
    for (auto* a : e.decs_get())
      {
        if (i++ != 0)
          ostr_ << misc::iendl;
        this->operator()(*a);
      }
  }

  void PrettyPrinter::operator()(const TypeDec& e)
  {
    ostr_ << "type " << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e << " */ ";
    ostr_ << " = " << e.ty_get();
  }

  void PrettyPrinter::operator()(const VarChunk& e)
  {
    size_t i = 0;
    for (auto* a : e.decs_get())
      {
        if (i++ != 0 && a->init_get() == nullptr)
          ostr_ << ", ";
        else
          ostr_ << misc::iendl;
        this->operator()(*a);
      }
  }

  void PrettyPrinter::operator()(const VarDec& e)
  {
    if (e.init_get() != nullptr)
      ostr_ << "var ";
    if (escapes_display(ostr_) && e.get_escapable())
      ostr_ << "/* escaping */ ";
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e << " */";
    if (e.type_name_get() != nullptr)
      ostr_ << " : " << *e.type_name_get();
    if (e.init_get() != nullptr)
      ostr_ << " := " << *e.init_get();
  }

  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while ";
    if (bindings_display(ostr_))
      ostr_ << "/* " << &e << " */ ";
    ostr_ << e.test_get() << " do " << e.body_get();
  }

} // namespace ast
