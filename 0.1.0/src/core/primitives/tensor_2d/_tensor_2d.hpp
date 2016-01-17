// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Tensor2D
// Description
//   Templated 2D tensor derived from VectorSpace adding construction from
//   4 components, element access using xx(), xy(), yx() and yy() member
//   functions and the iner-product (dot-product) and outer-product of two
//   Vector2Ds (tensor-product) operators.
#ifndef _tensor2_d_hpp_
#define _tensor2_d_hpp_
#include "_vector_2d.hpp"
#include "_spherical_tensor_2d.hpp"
namespace mousse
{
template<class Cmpt>
class SymmTensor2D;
template<class Cmpt>
class Tensor2D
:
  public VectorSpace<Tensor2D<Cmpt>, Cmpt, 4>
{
public:
  // Member constants
    enum
    {
      rank = 2 // Rank of Tensor2D is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const Tensor2D zero;
    static const Tensor2D one;
    static const Tensor2D max;
    static const Tensor2D min;
    static const Tensor2D I;
  //- Component labeling enumeration
  enum components { XX, XY, YX, YY };
  // Constructors
    //- Construct null
    inline Tensor2D();
    //- Construct given VectorSpace
    inline Tensor2D(const VectorSpace<Tensor2D<Cmpt>, Cmpt, 4>&);
    //- Construct given SymmTensor2D
    inline Tensor2D(const SymmTensor2D<Cmpt>&);
    //- Construct given SphericalTensor2D
    inline Tensor2D(const SphericalTensor2D<Cmpt>&);
    //- Construct given the two vectors
    inline Tensor2D
    (
      const Vector2D<Cmpt>& x,
      const Vector2D<Cmpt>& y
    );
    //- Construct given the four components
    inline Tensor2D
    (
      const Cmpt txx, const Cmpt txy,
      const Cmpt tyx, const Cmpt tyy
    );
    //- Construct from Istream
    Tensor2D(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& xx() const;
      inline const Cmpt& xy() const;
      inline const Cmpt& yx() const;
      inline const Cmpt& yy() const;
      inline Cmpt& xx();
      inline Cmpt& xy();
      inline Cmpt& yx();
      inline Cmpt& yy();
      // Access vector components.
      inline Vector2D<Cmpt> x() const;
      inline Vector2D<Cmpt> y() const;
    //- Transpose
    inline Tensor2D<Cmpt> T() const;
  // Member Operators
    //- Copy SymmTensor2D
    inline void operator=(const SymmTensor2D<Cmpt>&);
    //- Copy SphericalTensor2D
    inline void operator=(const SphericalTensor2D<Cmpt>&);
};
}  // namespace mousse

namespace mousse
{
// Constructors 
template<class Cmpt>
inline Tensor2D<Cmpt>::Tensor2D()
{}
template<class Cmpt>
inline Tensor2D<Cmpt>::Tensor2D(const VectorSpace<Tensor2D<Cmpt>, Cmpt, 4>& vs)
:
  VectorSpace<Tensor2D<Cmpt>, Cmpt, 4>{vs}
{}
template<class Cmpt>
inline Tensor2D<Cmpt>::Tensor2D(const SymmTensor2D<Cmpt>& st)
{
  this->v_[XX] = st.xx(); this->v_[XY] = st.xy();
  this->v_[YX] = st.xy(); this->v_[YY] = st.yy();
}
template<class Cmpt>
inline Tensor2D<Cmpt>::Tensor2D(const SphericalTensor2D<Cmpt>& st)
{
  this->v_[XX] = st.ii(); this->v_[XY] = 0;
  this->v_[YX] = 0; this->v_[YY] = st.ii();
}
template<class Cmpt>
inline Tensor2D<Cmpt>::Tensor2D
(
  const Vector2D<Cmpt>& x,
  const Vector2D<Cmpt>& y
)
{
  this->v_[XX] = x.x(); this->v_[XY] = x.y();
  this->v_[YX] = y.x(); this->v_[YY] = y.y();
}
template<class Cmpt>
inline Tensor2D<Cmpt>::Tensor2D
(
  const Cmpt txx, const Cmpt txy,
  const Cmpt tyx, const Cmpt tyy
)
{
  this->v_[XX] = txx; this->v_[XY] = txy;
  this->v_[YX] = tyx; this->v_[YY] = tyy;
}
template<class Cmpt>
inline Tensor2D<Cmpt>::Tensor2D(Istream& is)
:
  VectorSpace<Tensor2D<Cmpt>, Cmpt, 4>{is}
{}
// Member Functions 
template<class Cmpt>
inline Vector2D<Cmpt> Tensor2D<Cmpt>::x() const
{
  return {this->v_[XX], this->v_[XY]};
}
template<class Cmpt>
inline Vector2D<Cmpt> Tensor2D<Cmpt>::y() const
{
  return {this->v_[YX], this->v_[YY]};
}
template<class Cmpt>
inline const Cmpt& Tensor2D<Cmpt>::xx() const
{
  return this->v_[XX];
}
template<class Cmpt>
inline const Cmpt& Tensor2D<Cmpt>::xy() const
{
  return this->v_[XY];
}
template<class Cmpt>
inline const Cmpt& Tensor2D<Cmpt>::yx() const
{
  return this->v_[YX];
}
template<class Cmpt>
inline const Cmpt& Tensor2D<Cmpt>::yy() const
{
  return this->v_[YY];
}
template<class Cmpt>
inline Cmpt& Tensor2D<Cmpt>::xx()
{
  return this->v_[XX];
}
template<class Cmpt>
inline Cmpt& Tensor2D<Cmpt>::xy()
{
  return this->v_[XY];
}
template<class Cmpt>
inline Cmpt& Tensor2D<Cmpt>::yx()
{
  return this->v_[YX];
}
template<class Cmpt>
inline Cmpt& Tensor2D<Cmpt>::yy()
{
  return this->v_[YY];
}
template<class Cmpt>
inline Tensor2D<Cmpt> Tensor2D<Cmpt>::T() const
{
  return {xx(), yx(),
          xy(), yy()};
}
// Member Functions 
template<class Cmpt>
inline void Tensor2D<Cmpt>::operator=(const SymmTensor2D<Cmpt>& st)
{
  this->v_[XX] = st.xx(); this->v_[XY] = st.xy();
  this->v_[YX] = st.xy(); this->v_[YY] = st.yy();
}
template<class Cmpt>
inline void Tensor2D<Cmpt>::operator=(const SphericalTensor2D<Cmpt>& st)
{
  this->v_[XX] = st.ii(); this->v_[XY] = 0;
  this->v_[YX] = 0;       this->v_[YY] = st.ii();
}
// Global Operators 
//- Inner-product between two tensors
template<class Cmpt>
inline typename innerProduct<Tensor2D<Cmpt>, Tensor2D<Cmpt> >::type
operator&(const Tensor2D<Cmpt>& t1, const Tensor2D<Cmpt>& t2)
{
  return {t1.xx()*t2.xx() + t1.xy()*t2.yx(),
          t1.xx()*t2.xy() + t1.xy()*t2.yy(),
          t1.yx()*t2.xx() + t1.yy()*t2.yx(),
          t1.yx()*t2.xy() + t1.yy()*t2.yy()};
}
//- Inner-product between a tensor and a vector
template<class Cmpt>
inline typename innerProduct<Tensor2D<Cmpt>, Vector2D<Cmpt> >::type
operator&(const Tensor2D<Cmpt>& t, const Vector2D<Cmpt>& v)
{
  return {t.xx()*v.x() + t.xy()*v.y(),
          t.yx()*v.x() + t.yy()*v.y()};
}
//- Inner-product between a vector and a tensor
template<class Cmpt>
inline typename innerProduct<Vector2D<Cmpt>, Tensor2D<Cmpt> >::type
operator&(const Vector2D<Cmpt>& v, const Tensor2D<Cmpt>& t)
{
  return {v.x()*t.xx() + v.y()*t.yx(),
          v.x()*t.xy() + v.y()*t.yy()};
}
//- Outer-product between two vectors
template<class Cmpt>
inline typename outerProduct<Vector2D<Cmpt>, Vector2D<Cmpt> >::type
operator*(const Vector2D<Cmpt>& v1, const Vector2D<Cmpt>& v2)
{
  return {v1.x()*v2.x(), v1.x()*v2.y(),
          v1.y()*v2.x(), v1.y()*v2.y()};
}
//- Return the trace of a tensor
template<class Cmpt>
inline Cmpt tr(const Tensor2D<Cmpt>& t)
{
  return t.xx() + t.yy();
}
//- Return the spherical part of a tensor
template<class Cmpt>
inline SphericalTensor2D<Cmpt> sph(const Tensor2D<Cmpt>& t)
{
  return 0.5*tr(t);
}
//- Return the symmetric part of a tensor
template<class Cmpt>
inline SymmTensor2D<Cmpt> symm(const Tensor2D<Cmpt>& t)
{
  return {t.xx(), 0.5*(t.xy() + t.yx()), t.yy()};
}
//- Return the twice the symmetric part of a tensor
template<class Cmpt>
inline SymmTensor2D<Cmpt> twoSymm(const Tensor2D<Cmpt>& t)
{
  return {t.xx() + t.xx(), t.xy() + t.yx(), t.yy() + t.yy()};
}
//- Return the skew-symmetric part of a tensor
template<class Cmpt>
inline Tensor2D<Cmpt> skew(const Tensor2D<Cmpt>& t)
{
  return {0.0, 0.5*(t.xy() - t.yx()), 0.5*(t.yx() - t.xy()), 0.0};
}
//- Return the deviatoric part of a tensor
template<class Cmpt>
inline Tensor2D<Cmpt> dev(const Tensor2D<Cmpt>& t)
{
  return t - SphericalTensor2D<Cmpt>::oneThirdI*tr(t);
}
//- Return the deviatoric part of a tensor
template<class Cmpt>
inline Tensor2D<Cmpt> dev2(const Tensor2D<Cmpt>& t)
{
  return t - SphericalTensor2D<Cmpt>::twoThirdsI*tr(t);
}
//- Return the determinant of a tensor
template<class Cmpt>
inline Cmpt det(const Tensor2D<Cmpt>& t)
{
  return t.xx()*t.yy() - t.xy()*t.yx();
}
//- Return the cofactor tensor of a tensor
template<class Cmpt>
inline Tensor2D<Cmpt> cof(const Tensor2D<Cmpt>& t)
{
  return {t.yy(), -t.xy(),
          -t.yx(),  t.xx()};
}
//- Return the inverse of a tensor given the determinant
template<class Cmpt>
inline Tensor2D<Cmpt> inv(const Tensor2D<Cmpt>& t, const Cmpt dett)
{
  return cof(t)/dett;
}
//- Return the inverse of a tensor
template<class Cmpt>
inline Tensor2D<Cmpt> inv(const Tensor2D<Cmpt>& t)
{
  return inv(t, det(t));
}
//- Return the 1st invariant of a tensor
template<class Cmpt>
inline Cmpt invariantI(const Tensor2D<Cmpt>& t)
{
  return tr(t);
}
//- Return the 2nd invariant of a tensor
template<class Cmpt>
inline Cmpt invariantII(const Tensor2D<Cmpt>& t)
{
  return 0.5*sqr(tr(t)) - 0.5*(t.xx()*t.xx() + t.xy()*t.xy()
                               + t.yx()*t.yx() + t.yy()*t.yy());
}
//- Return the 3rd invariant of a tensor
template<class Cmpt>
inline Cmpt invariantIII(const Tensor2D<Cmpt>& t)
{
  return det(t);
}
// Mixed Tensor SphericalTensor Operators 
template<class Cmpt>
inline Tensor2D<Cmpt>
operator+(const SphericalTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return {st1.ii() + t2.xx(), t2.xy(),
          t2.yx(),            st1.ii() + t2.yy()};
}
template<class Cmpt>
inline Tensor2D<Cmpt>
operator+(const Tensor2D<Cmpt>& t1, const SphericalTensor2D<Cmpt>& st2)
{
  return {t1.xx() + st2.ii(), t1.xy(),
          t1.yx(),            t1.yy() + st2.ii()};
}
template<class Cmpt>
inline Tensor2D<Cmpt>
operator-(const SphericalTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return {st1.ii() - t2.xx(), -t2.xy(),
          -t2.yx(),           st1.ii() - t2.yy()};
}
template<class Cmpt>
inline Tensor2D<Cmpt>
operator-(const Tensor2D<Cmpt>& t1, const SphericalTensor2D<Cmpt>& st2)
{
  return {t1.xx() - st2.ii(), t1.xy(),
          t1.yx(),            t1.yy() - st2.ii()};
}
//- Inner-product between a spherical tensor and a tensor
template<class Cmpt>
inline Tensor2D<Cmpt>
operator&(const SphericalTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return {st1.ii()*t2.xx(),
          st1.ii()*t2.xy(),
          st1.ii()*t2.yx(),
          st1.ii()*t2.yy()};
}
//- Inner-product between a tensor and a spherical tensor
template<class Cmpt>
inline Tensor2D<Cmpt>
operator&(const Tensor2D<Cmpt>& t1, const SphericalTensor2D<Cmpt>& st2)
{
  return {t1.xx()*st2.ii(),
          t1.xy()*st2.ii(),
          t1.yx()*st2.ii(),
          t1.yy()*st2.ii()};
}
//- Double-dot-product between a spherical tensor and a tensor
template<class Cmpt>
inline Cmpt
operator&&(const SphericalTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return st1.ii()*t2.xx() + st1.ii()*t2.yy();
}
//- Double-dot-product between a tensor and a spherical tensor
template<class Cmpt>
inline Cmpt
operator&&(const Tensor2D<Cmpt>& t1, const SphericalTensor2D<Cmpt>& st2)
{
  return t1.xx()*st2.ii() + t1.yy()*st2.ii();
}
// Mixed Tensor SymmTensor Operators
template<class Cmpt>
inline Tensor2D<Cmpt>
operator+(const SymmTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return {st1.xx() + t2.xx(), st1.xy() + t2.xy(),
          st1.xy() + t2.yx(), st1.yy() + t2.yy()};
}
template<class Cmpt>
inline Tensor2D<Cmpt>
operator+(const Tensor2D<Cmpt>& t1, const SymmTensor2D<Cmpt>& st2)
{
  return {t1.xx() + st2.xx(), t1.xy() + st2.xy(),
          t1.yx() + st2.xy(), t1.yy() + st2.yy()};
}
template<class Cmpt>
inline Tensor2D<Cmpt>
operator-(const SymmTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return {st1.xx() - t2.xx(), st1.xy() - t2.xy(),
          st1.xy() - t2.yx(), st1.yy() - t2.yy()};
}
template<class Cmpt>
inline Tensor2D<Cmpt>
operator-(const Tensor2D<Cmpt>& t1, const SymmTensor2D<Cmpt>& st2)
{
  return {t1.xx() - st2.xx(), t1.xy() - st2.xy(),
          t1.yx() - st2.xy(), t1.yy() - st2.yy()};
}
//- Inner-product between a spherical tensor and a tensor
template<class Cmpt>
inline Tensor2D<Cmpt>
operator&(const SymmTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return {st1.xx()*t2.xx() + st1.xy()*t2.yx(),
          st1.xx()*t2.xy() + st1.xy()*t2.yy(),
          st1.xy()*t2.xx() + st1.yy()*t2.yx(),
          st1.xy()*t2.xy() + st1.yy()*t2.yy()};
}
//- Inner-product between a tensor and a spherical tensor
template<class Cmpt>
inline Tensor2D<Cmpt>
operator&(const Tensor2D<Cmpt>& t1, const SymmTensor2D<Cmpt>& st2)
{
  return {t1.xx()*st2.xx() + t1.xy()*st2.xy(),
          t1.xx()*st2.xy() + t1.xy()*st2.yy(),
          t1.yx()*st2.xx() + t1.yy()*st2.xy(),
          t1.yx()*st2.xy() + t1.yy()*st2.yy()};
}
//- Double-dot-product between a spherical tensor and a tensor
template<class Cmpt>
inline Cmpt
operator&&(const SymmTensor2D<Cmpt>& st1, const Tensor2D<Cmpt>& t2)
{
  return st1.xx()*t2.xx() + st1.xy()*t2.xy() + st1.xy()*t2.yx()
    + st1.yy()*t2.yy();
}
//- Double-dot-product between a tensor and a spherical tensor
template<class Cmpt>
inline Cmpt
operator&&(const Tensor2D<Cmpt>& t1, const SymmTensor2D<Cmpt>& st2)
{
  return t1.xx()*st2.xx() + t1.xy()*st2.xy() + t1.yx()*st2.xy()
    + t1.yy()*st2.yy();
}
template<class Cmpt>
class typeOfSum<SphericalTensor2D<Cmpt>, Tensor2D<Cmpt> >
{
public:
  typedef Tensor2D<Cmpt> type;
};
template<class Cmpt>
class typeOfSum<Tensor2D<Cmpt>, SphericalTensor2D<Cmpt> >
{
public:
  typedef Tensor2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<Tensor2D<Cmpt>, Tensor2D<Cmpt> >
{
public:
  typedef Tensor2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<SphericalTensor2D<Cmpt>, Tensor2D<Cmpt> >
{
public:
  typedef Tensor2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<Tensor2D<Cmpt>, SphericalTensor2D<Cmpt> >
{
public:
  typedef Tensor2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<Tensor2D<Cmpt>, Vector2D<Cmpt> >
{
public:
  typedef Vector2D<Cmpt> type;
};
template<class Cmpt>
class innerProduct<Vector2D<Cmpt>, Tensor2D<Cmpt> >
{
public:
  typedef Vector2D<Cmpt> type;
};
template<class Cmpt>
class outerProduct<Vector2D<Cmpt>, Vector2D<Cmpt> >
{
public:
  typedef Tensor2D<Cmpt> type;
};
}  // namespace mousse
#endif
