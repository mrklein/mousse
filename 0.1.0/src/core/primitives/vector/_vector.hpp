// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Vector
// Description
//   Templated 3D Vector derived from VectorSpace adding construction from
//   3 components, element access using x(), y() and z() member functions and
//   the inner-product (dot-product) and cross product operators.
//   A centre() member function which returns the Vector for which it is called
//   is defined so that point which is a typedef to Vector\<scalar\> behaves as
//   other shapes in the shape hierachy.
// SourceFiles
//   _vector_i.hpp
#ifndef _vector_hpp_
#define _vector_hpp_
#include "vector_space.hpp"
namespace mousse
{
template<class T> class List;
template<class Cmpt>
class Vector
:
  public VectorSpace<Vector<Cmpt>, Cmpt, 3>
{
public:
  //- Equivalent type of labels used for valid component indexing
  typedef Vector<label> labelType;
  // Member constants
    enum
    {
      rank = 1 // Rank of Vector is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const Vector zero;
    static const Vector one;
    static const Vector max;
    static const Vector min;
    static const Vector rootMax;
    static const Vector rootMin;
  //- Component labeling enumeration
  enum components { X, Y, Z };
  // Constructors
    //- Construct null
    inline Vector();
    //- Construct given VectorSpace of the same rank
    template<class Cmpt2>
    inline Vector(const VectorSpace<Vector<Cmpt2>, Cmpt2, 3>&);
    //- Construct given three components
    inline Vector(const Cmpt& vx, const Cmpt& vy, const Cmpt& vz);
    //- Construct from Istream
    inline Vector(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& x() const;
      inline const Cmpt& y() const;
      inline const Cmpt& z() const;
      inline Cmpt& x();
      inline Cmpt& y();
      inline Cmpt& z();
    //- Return *this (used for point which is a typedef to Vector<scalar>.
    inline const Vector<Cmpt>& centre
    (
      const mousse::List<Vector<Cmpt> >&
    ) const;
};
template<class Cmpt>
class typeOfRank<Cmpt, 1>
{
public:
  typedef Vector<Cmpt> type;
};
template<class Cmpt>
class symmTypeOfRank<Cmpt, 1>
{
public:
  typedef Vector<Cmpt> type;
};
}  // namespace mousse
#include "_vector_i.hpp"
#endif
