// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SphericalTensor2D
// Description
//   Templated 2D sphericalTensor derived from VectorSpace adding construction
//   from 1 component, element access using ii() member function and the
//   inner-product (dot-product) and outer-product operators.
#ifndef _spherical_tensor_2d_hpp_
#define _spherical_tensor_2d_hpp_
#include "vector_space.hpp"
#include "vector_2d.hpp"
namespace mousse
{
template<class Cmpt>
class SphericalTensor2D
:
  public VectorSpace<SphericalTensor2D<Cmpt>, Cmpt, 1>
{
public:
  // Member constants
    enum
    {
      rank = 2 // Rank of SphericalTensor2D is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const SphericalTensor2D zero;
    static const SphericalTensor2D one;
    static const SphericalTensor2D max;
    static const SphericalTensor2D min;
    static const SphericalTensor2D I;
    static const SphericalTensor2D oneThirdI;
    static const SphericalTensor2D twoThirdsI;
  //- Component labeling enumeration
  enum components { II };
  // Constructors
    //- Construct null
    inline SphericalTensor2D();
    //- Construct given VectorSpace
    inline SphericalTensor2D
    (
      const VectorSpace<SphericalTensor2D<Cmpt>, Cmpt, 1>&
    );
    //- Construct given the component
    inline SphericalTensor2D(const Cmpt& tii);
    //- Construct from Istream
    inline SphericalTensor2D(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& ii() const;
      inline Cmpt& ii();
};
}  // namespace mousse

namespace mousse
{
// Constructors 
// Construct null
template<class Cmpt>
inline SphericalTensor2D<Cmpt>::SphericalTensor2D()
{}
// Construct given VectorSpace
template<class Cmpt>
inline SphericalTensor2D<Cmpt>::SphericalTensor2D
(
  const VectorSpace<SphericalTensor2D<Cmpt>, Cmpt, 1>& vs
)
:
  VectorSpace<SphericalTensor2D<Cmpt>, Cmpt, 1>{vs}
{}
// Construct given three Cmpts
template<class Cmpt>
inline SphericalTensor2D<Cmpt>::SphericalTensor2D(const Cmpt& stii)
{
  this->v_[II] = stii;
}
// Construct from Istream
template<class Cmpt>
inline SphericalTensor2D<Cmpt>::SphericalTensor2D(Istream& is)
:
  VectorSpace<SphericalTensor2D<Cmpt>, Cmpt, 1>(is)
{}
// Member Functions 
template<class Cmpt>
inline const Cmpt&  SphericalTensor2D<Cmpt>::ii() const
{
  return this->v_[II];
}
template<class Cmpt>
inline Cmpt& SphericalTensor2D<Cmpt>::ii()
{
  return this->v_[II];
}
// Global Operators 
//- Inner-product between two spherical tensors
template<class Cmpt>
inline SphericalTensor2D<Cmpt>
operator&
(
  const SphericalTensor2D<Cmpt>& st1,
  const SphericalTensor2D<Cmpt>& st2
)
{
  return SphericalTensor2D<Cmpt>{st1.ii()*st2.ii()};
}
//- Inner-product between a spherical tensor and a vector
template<class Cmpt>
inline Vector2D<Cmpt>
operator&(const SphericalTensor2D<Cmpt>& st, const Vector2D<Cmpt>& v)
{
  return Vector2D<Cmpt>
  {
    st.ii()*v.x(),
           st.ii()*v.y()
  };
}
//- Inner-product between a vector and a spherical tensor
template<class Cmpt>
inline Vector2D<Cmpt>
operator&(const Vector2D<Cmpt>& v, const SphericalTensor2D<Cmpt>& st)
{
  return Vector2D<Cmpt>
  {
    v.x()*st.ii(),
           v.y()*st.ii()
  };
}
//- Division of a scalar by a sphericalTensor2D
template<class Cmpt>
inline SphericalTensor2D<Cmpt>
operator/(const scalar s, const SphericalTensor2D<Cmpt>& st)
{
  return SphericalTensor2D<Cmpt>{s/st.ii()};
}
//- Return the trace of a spherical tensor
template<class Cmpt>
inline Cmpt tr(const SphericalTensor2D<Cmpt>& st)
{
  return 2*st.ii();
}
//- Return the spherical part of a spherical tensor, i.e. itself
template<class Cmpt>
inline SphericalTensor2D<Cmpt> sph(const SphericalTensor2D<Cmpt>& st)
{
  return st;
}
//- Return the determinant of a spherical tensor
template<class Cmpt>
inline Cmpt det(const SphericalTensor2D<Cmpt>& st)
{
  return st.ii()*st.ii();
}
//- Return the inverse of a symmetric tensor
template<class Cmpt>
inline SphericalTensor2D<Cmpt> inv(const SphericalTensor2D<Cmpt>& st)
{
  return SphericalTensor2D<Cmpt>(1.0/st.ii());
}
template<class Cmpt>
class outerProduct<SphericalTensor2D<Cmpt>, Cmpt>
{
public:
  typedef SphericalTensor2D<Cmpt> type;
};
template<class Cmpt>
class outerProduct<Cmpt, SphericalTensor2D<Cmpt> >
{
public:
  typedef SphericalTensor2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<SphericalTensor2D<Cmpt>, SphericalTensor2D<Cmpt> >
{
public:
  typedef SphericalTensor2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<SphericalTensor2D<Cmpt>, Vector2D<Cmpt> >
{
public:
  typedef Vector2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<Vector2D<Cmpt>, SphericalTensor2D<Cmpt> >
{
public:
  typedef Vector2D<Cmpt> type;
};
}  // namespace mousse
#endif
