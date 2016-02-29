#ifndef CORE_PRIMITIVES_SPHERICAL_TENSOR_TSPHERICAL_TENSOR_HPP_
#define CORE_PRIMITIVES_SPHERICAL_TENSOR_TSPHERICAL_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SphericalTensor
// Description
//   Templated 3D SphericalTensor derived from VectorSpace adding construction
//   from 1 component, element access using th ii() member function and the
//   inner-product (dot-product) and outer-product operators.
#include "vector_space.hpp"
#include "vector.hpp"
namespace mousse
{
template<class Cmpt>
class SphericalTensor
:
  public VectorSpace<SphericalTensor<Cmpt>, Cmpt, 1>
{
public:
  //- Equivalent type of labels used for valid component indexing
  typedef SphericalTensor<label> labelType;
  // Member constants
    enum
    {
      rank = 2 // Rank of SphericalTensor is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const SphericalTensor zero;
    static const SphericalTensor one;
    static const SphericalTensor max;
    static const SphericalTensor min;
    static const SphericalTensor I;
    static const SphericalTensor oneThirdI;
    static const SphericalTensor twoThirdsI;
  //- Component labeling enumeration
  enum components { II };
  // Constructors
    //- Construct null
    inline SphericalTensor();
    //- Construct given VectorSpace
    template<class Cmpt2>
    inline SphericalTensor
    (
      const VectorSpace<SphericalTensor<Cmpt2>, Cmpt2, 1>&
    );
    //- Construct given the component
    inline SphericalTensor(const Cmpt& tii);
    //- Construct from Istream
    inline SphericalTensor(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& ii() const;
      inline Cmpt& ii();
    //- Transpose
    inline const SphericalTensor<Cmpt>& T() const;
};
}  // namespace mousse

namespace mousse
{
// Constructors 
template<class Cmpt>
inline SphericalTensor<Cmpt>::SphericalTensor()
{}
template<class Cmpt>
template<class Cmpt2>
inline SphericalTensor<Cmpt>::SphericalTensor
(
  const VectorSpace<SphericalTensor<Cmpt2>, Cmpt2, 1>& vs
)
:
  VectorSpace<SphericalTensor<Cmpt>, Cmpt, 1>{vs}
{}
template<class Cmpt>
inline SphericalTensor<Cmpt>::SphericalTensor(const Cmpt& stii)
{
  this->v_[II] = stii;
}
template<class Cmpt>
inline SphericalTensor<Cmpt>::SphericalTensor(Istream& is)
:
  VectorSpace<SphericalTensor<Cmpt>, Cmpt, 1>(is)
{}
// Member Functions 
template<class Cmpt>
inline const Cmpt&  SphericalTensor<Cmpt>::ii() const
{
  return this->v_[II];
}
template<class Cmpt>
inline Cmpt& SphericalTensor<Cmpt>::ii()
{
  return this->v_[II];
}
template<class Cmpt>
inline const SphericalTensor<Cmpt>& SphericalTensor<Cmpt>::T() const
{
  return *this;
}
// Global Operators 
//- Inner-product between two spherical tensors
template<class Cmpt>
inline SphericalTensor<Cmpt>
operator&(const SphericalTensor<Cmpt>& st1, const SphericalTensor<Cmpt>& st2)
{
  return SphericalTensor<Cmpt>{st1.ii()*st2.ii()};
}
//- Inner-product between a spherical tensor and a vector
template<class Cmpt>
inline Vector<Cmpt>
operator&(const SphericalTensor<Cmpt>& st, const Vector<Cmpt>& v)
{
  return Vector<Cmpt>
  {
    st.ii()*v.x(),
           st.ii()*v.y(),
                   st.ii()*v.z()
  };
}
//- Inner-product between a vector and a spherical tensor
template<class Cmpt>
inline Vector<Cmpt>
operator&(const Vector<Cmpt>& v, const SphericalTensor<Cmpt>& st)
{
  return Vector<Cmpt>
  {
    v.x()*st.ii(),
           v.y()*st.ii(),
                   v.z()*st.ii()
  };
}
//- Double-dot-product between a spherical tensor and a spherical tensor
template<class Cmpt>
inline Cmpt
operator&&(const SphericalTensor<Cmpt>& st1, const SphericalTensor<Cmpt>& st2)
{
  return 3*st1.ii()*st2.ii();
}
//- Division of a scalar by a sphericalTensor
template<class Cmpt>
inline SphericalTensor<Cmpt>
operator/(const scalar s, const SphericalTensor<Cmpt>& st)
{
  return SphericalTensor<Cmpt>{s/st.ii()};
}
template<class Cmpt>
inline Cmpt magSqr(const SphericalTensor<Cmpt>& st)
{
  return 3*magSqr(st.ii());
}
//- Return the trace of a spherical tensor
template<class Cmpt>
inline Cmpt tr(const SphericalTensor<Cmpt>& st)
{
  return 3*st.ii();
}
//- Return the spherical part of a spherical tensor, i.e. itself
template<class Cmpt>
inline SphericalTensor<Cmpt> sph(const SphericalTensor<Cmpt>& st)
{
  return st;
}
//- Return the determinant of a spherical tensor
template<class Cmpt>
inline Cmpt det(const SphericalTensor<Cmpt>& st)
{
  return st.ii()*st.ii()*st.ii();
}
//- Return the inverse of a spherical tensor
template<class Cmpt>
inline SphericalTensor<Cmpt> inv(const SphericalTensor<Cmpt>& st)
{
  return SphericalTensor<Cmpt>{1.0/st.ii()};
}
template<class Cmpt>
class outerProduct<SphericalTensor<Cmpt>, Cmpt>
{
public:
  typedef SphericalTensor<Cmpt> type;
};
template<class Cmpt>
class outerProduct<Cmpt, SphericalTensor<Cmpt> >
{
public:
  typedef SphericalTensor<Cmpt> type;
};
template<class Cmpt>
class innerProduct<SphericalTensor<Cmpt>, SphericalTensor<Cmpt> >
{
public:
  typedef SphericalTensor<Cmpt> type;
};
template<class Cmpt>
class innerProduct<SphericalTensor<Cmpt>, Vector<Cmpt> >
{
public:
  typedef Vector<Cmpt> type;
};
template<class Cmpt>
class innerProduct<Vector<Cmpt>, SphericalTensor<Cmpt> >
{
public:
  typedef Vector<Cmpt> type;
};
}  // namespace mousse
#endif
