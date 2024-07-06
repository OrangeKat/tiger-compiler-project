/**
 ** \file type/named.cc
 ** \brief Implementation for type/named.hh.
 */

#include <set>

#include <type/named.hh>
#include <type/visitor.hh>

namespace type
{
  Named::Named(misc::symbol name)
    : name_(name)
    , type_(nullptr)
  {}

  Named::Named(misc::symbol name, const Type* type)
    : name_(name)
    , type_(type)
  {}

  // Inherited functions
  void Named::accept(ConstVisitor& v) const
  {
    // FIXME: Some code was deleted here.
    v(*this);
  }

  void Named::accept(Visitor& v)
  {
    // FIXME: Some code was deleted here.
    v(*this);
  }

  bool Named::sound() const
  {
    // FIXME: Some code was deleted here (Sound).
    std::set<const Type*> set;
    auto curr = this;
    while (curr)
      {
        if (!set.insert(curr).second)
          {
            return false;
          }
        curr = dynamic_cast<const Named*>(curr->type_);
      }
    return true;
  }

  bool Named::compatible_with(const Type& other) const
  {
    // FIXME: Some code was deleted here (Special implementation of "compatible_with" for Named).
    return type_->compatible_with(other);
  }

} // namespace type
