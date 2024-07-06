/**
 ** \file type/array.hh
 ** \brief The class Array.
 */
#pragma once

#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
  /// Array types.
  class Array : public Type
  {
    // FIXME: Some code was deleted here.
  public:
    Array(const Type& type);
    virtual ~Array() = default;

    void accept(Visitor& v) override;
    void accept(ConstVisitor& v) const override;

    const Type& type_get() const;

  private:
    const Type& type_;
  };

} // namespace type

#include <type/array.hxx>
