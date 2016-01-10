// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Vector2D
// Description
//   Templated 2D Vector derived from VectorSpace adding construction from
//   2 components, element access using x() and y() member functions and
//   the inner-product (dot-product).
// SourceFiles
//   _vector_2d_i.hpp
#ifndef _vector_2d_hpp_
#define _vector_2d_hpp_
#include "vector_space.hpp"
namespace mousse
{
template<class Cmpt>
class Vector2D
:
  public VectorSpace<Vector2D<Cmpt>, Cmpt, 2>
{
public:
  // Member constants
    enum
    {
      rank = 1 // Rank of Vector2D is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const Vector2D zero;
    static const Vector2D one;
    static const Vector2D max;
    static const Vector2D min;
  //- Component labeling enumeration
  enum components { X, Y };
  // Constructors
    //- Construct null
    inline Vector2D();
    //- Construct given VectorSpace
    inline Vector2D(const VectorSpace<Vector2D<Cmpt>, Cmpt, 2>&);
    //- Construct given two components
    inline Vector2D(const Cmpt& vx, const Cmpt& vy);
    //- Construct from Istream
    inline Vector2D(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& x() const;
      inline const Cmpt& y() const;
      inline Cmpt& x();
      inline Cmpt& y();
    // Operators
      //- Perp dot product (dot product with perpendicular vector)
      inline scalar perp(const Vector2D<Cmpt>& b) const;
};
}  // namespace mousse
// Include inline implementations
#include "_vector_2d_i.hpp"
#endif
