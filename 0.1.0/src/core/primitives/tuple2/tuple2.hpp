// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Tuple2
// Description
//   A 2-tuple for storing two objects of different types.
// SeeAlso
//   mousse::Pair for storing two objects of identical types.
#ifndef tuple2_hpp_
#define tuple2_hpp_
#include "istream.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Type1, class Type2>
class Tuple2;
template<class Type1, class Type2>
inline Istream& operator>>(Istream&, Tuple2<Type1, Type2>&);
template<class Type1, class Type2>
inline Ostream& operator<<(Ostream&, const Tuple2<Type1, Type2>&);
template<class Type1, class Type2>
class Tuple2
{
  // Private data
    Type1 f_;
    Type2 s_;
public:
  // Constructors
    //- Null constructor for lists
    inline Tuple2()
    {}
    //- Construct from components
    inline Tuple2(const Type1& f, const Type2& s)
    :
      f_(f),
      s_(s)
    {}
    //- Construct from Istream
    inline Tuple2(Istream& is)
    {
      is >> *this;
    }
  // Member Functions
    //- Return first
    inline const Type1& first() const
    {
      return f_;
    }
    //- Return first
    inline Type1& first()
    {
      return f_;
    }
    //- Return second
    inline const Type2& second() const
    {
      return s_;
    }
    //- Return second
    inline Type2& second()
    {
      return s_;
    }
  // IOstream operators
    //- Read Tuple2 from Istream, discarding contents of existing Tuple2.
    friend Istream& operator>> <Type1, Type2>
    (
      Istream& is,
      Tuple2<Type1, Type2>& t2
    );
    // Write Tuple2 to Ostream.
    friend Ostream& operator<< <Type1, Type2>
    (
      Ostream& os,
      const Tuple2<Type1, Type2>& t2
    );
};
//- Return reverse of a tuple2
template<class Type1, class Type2>
inline Tuple2<Type2, Type1> reverse(const Tuple2<Type1, Type2>& t)
{
  return Tuple2<Type2, Type1>(t.second(), t.first());
}
template<class Type1, class Type2>
inline bool operator==
(
  const Tuple2<Type1, Type2>& a,
  const Tuple2<Type1, Type2>& b
)
{
  return (a.first() == b.first() && a.second() == b.second());
}
template<class Type1, class Type2>
inline bool operator!=
(
  const Tuple2<Type1, Type2>& a,
  const Tuple2<Type1, Type2>& b
)
{
  return !(a == b);
}
template<class Type1, class Type2>
inline Istream& operator>>(Istream& is, Tuple2<Type1, Type2>& t2)
{
  is.readBegin("Tuple2");
  is >> t2.f_ >> t2.s_;
  is.readEnd("Tuple2");
  // Check state of Istream
  is.check("operator>>(Istream&, Tuple2<Type1, Type2>&)");
  return is;
}
template<class Type1, class Type2>
inline Ostream& operator<<(Ostream& os, const Tuple2<Type1, Type2>& t2)
{
  os  << token::BEGIN_LIST
    << t2.f_ << token::SPACE << t2.s_
    << token::END_LIST;
  return os;
}
}  // namespace mousse
#endif
