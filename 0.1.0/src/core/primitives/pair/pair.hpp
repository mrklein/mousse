#ifndef CORE_PRIMITIVES_PAIR_PAIR_HPP_
#define CORE_PRIMITIVES_PAIR_PAIR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Pair
// Description
//   An ordered pair of two objects of type \<T\> with first() and second()
//   elements.
// SeeAlso
//   mousse::Tuple2 for storing two objects of dissimilar types.
#include "fixed_list.hpp"
#include "istream.hpp"
namespace mousse
{
template<class Type>
class Pair
:
  public FixedList<Type, 2>
{
public:
  // Constructors
    //- Null constructor
    inline Pair()
    {}
    //- Construct from components
    inline Pair(const Type& f, const Type& s)
    {
      first() = f;
      second() = s;
    }
    //- Construct from FixedList
    inline Pair(const FixedList<Type, 2>& fl)
    :
      FixedList<Type, 2>(fl)
    {}
    //- Construct from Istream
    inline Pair(Istream& is)
    :
      FixedList<Type, 2>(is)
    {}
  // Member Functions
    //- Return first
    inline const Type& first() const
    {
      return this->operator[](0);
    }
    //- Return first
    inline Type& first()
    {
      return this->operator[](0);
    }
    //- Return second
    inline const Type& second() const
    {
      return this->operator[](1);
    }
    //- Return second
    inline Type& second()
    {
      return this->operator[](1);
    }
    //- Return other
    inline const Type& other(const Type& a) const
    {
      if (first() == second())
      {
        FATAL_ERROR_IN("Pair<Type>::other(const Type&) const")
          << "Call to other only valid for Pair with differing"
          << " elements:" << *this << abort(FatalError);
      }
      else if (first() == a)
      {
        return second();
      }
      else
      {
        if (second() != a)
        {
          FATAL_ERROR_IN("Pair<Type>::other(const Type&) const")
            << "Pair " << *this
            << " does not contain " << a << abort(FatalError);
        }
        return first();
      }
    }
    //- Compare Pairs
    //  Returning:
    //  -  0: different
    //  - +1: identical
    //  - -1: same pair, but reversed order
    static inline int compare(const Pair<Type>& a, const Pair<Type>& b)
    {
      if (a == b)
      {
        return 1;
      }
      else if (a == reverse(b))
      {
        return -1;
      }
      else
      {
        return 0;
      }
    }
};
template<class Type>
Pair<Type> reverse(const Pair<Type>& p)
{
  return Pair<Type>(p.second(), p.first());
}
template<class Type>
bool operator==(const Pair<Type>& a, const Pair<Type>& b)
{
  return (a.first() == b.first() && a.second() == b.second());
}
template<class Type>
bool operator!=(const Pair<Type>& a, const Pair<Type>& b)
{
  return !(a == b);
}
template<class Type>
bool operator<(const Pair<Type>& a, const Pair<Type>& b)
{
  return
  (
    a.first() < b.first()
  ||
    (
      !(b.first() < a.first())
    && a.second() < b.second()
    )
  );
}
template<class Type>
bool operator<=(const Pair<Type>& a, const Pair<Type>& b)
{
  return !(b < a);
}
template<class Type>
bool operator>(const Pair<Type>& a, const Pair<Type>& b)
{
  return (b < a);
}
template<class Type>
bool operator>=(const Pair<Type>& a, const Pair<Type>& b)
{
  return !(a < b);
}
}  // namespace mousse
#endif
