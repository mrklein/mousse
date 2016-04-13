#ifndef CORE_PRIMITIVES_SYMM_TENSOR_TSYMM_TENSOR_HPP_
#define CORE_PRIMITIVES_SYMM_TENSOR_TSYMM_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SymmTensor
// Description
//   Templated 3D symmetric tensor derived from VectorSpace adding construction
//   from 6 components, element access using xx(), xy() etc. member functions
//   and the inner-product (dot-product) and outer-product of two Vectors
//   (tensor-product) operators.

#include "vector_space.hpp"
#include "_spherical_tensor.hpp"
#include "_vector.hpp"
#include "_tensor.hpp"


namespace mousse {

template<class Cmpt>
class SymmTensor
:
  public VectorSpace<SymmTensor<Cmpt>, Cmpt, 6>
{
public:
  //- Equivalent type of labels used for valid component indexing
  typedef SymmTensor<label> labelType;
  // Member constants
    enum {
      rank = 2 // Rank of SymmTensor is 2
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const SymmTensor zero;
    static const SymmTensor one;
    static const SymmTensor max;
    static const SymmTensor min;
    static const SymmTensor I;
  //- Component labeling enumeration
  enum components { XX, XY, XZ, YY, YZ, ZZ };
  // Constructors
    //- Construct null
    inline SymmTensor();
    //- Construct given VectorSpace of the same rank
    template<class Cmpt2>
    inline SymmTensor(const VectorSpace<SymmTensor<Cmpt2>, Cmpt2, 6>&);
    //- Construct given SphericalTensor
    inline SymmTensor(const SphericalTensor<Cmpt>&);
    //- Construct given the six components
    inline SymmTensor
    (
      const Cmpt txx, const Cmpt txy, const Cmpt txz,
                      const Cmpt tyy, const Cmpt tyz,
                                      const Cmpt tzz
    );
    //- Construct from Istream
    SymmTensor(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& xx() const;
      inline const Cmpt& xy() const;
      inline const Cmpt& xz() const;
      inline const Cmpt& yy() const;
      inline const Cmpt& yz() const;
      inline const Cmpt& zz() const;
      inline Cmpt& xx();
      inline Cmpt& xy();
      inline Cmpt& xz();
      inline Cmpt& yy();
      inline Cmpt& yz();
      inline Cmpt& zz();
    //- Transpose
    inline const SymmTensor<Cmpt>& T() const;
  // Member Operators
    //- Construct given SphericalTensor
    inline void operator=(const SphericalTensor<Cmpt>&);
};


template<class Cmpt>
class symmTypeOfRank<Cmpt, 2>
{
public:
  typedef SymmTensor<Cmpt> type;
};


// Constructors
template<class Cmpt>
inline SymmTensor<Cmpt>::SymmTensor()
{}


template<class Cmpt>
template<class Cmpt2>
inline SymmTensor<Cmpt>::SymmTensor
(
  const VectorSpace<SymmTensor<Cmpt2>, Cmpt2, 6>& vs
)
:
  VectorSpace<SymmTensor<Cmpt>, Cmpt, 6>{vs}
{}


template<class Cmpt>
inline SymmTensor<Cmpt>::SymmTensor(const SphericalTensor<Cmpt>& st)
{
  this->v_[XX] = st.ii(); this->v_[XY] = 0;       this->v_[XZ] = 0;
                          this->v_[YY] = st.ii(); this->v_[YZ] = 0;
                                                  this->v_[ZZ] = st.ii();
}


template<class Cmpt>
inline SymmTensor<Cmpt>::SymmTensor
(
  const Cmpt txx, const Cmpt txy, const Cmpt txz,
                  const Cmpt tyy, const Cmpt tyz,
                                  const Cmpt tzz
)
{
  this->v_[XX] = txx; this->v_[XY] = txy; this->v_[XZ] = txz;
                      this->v_[YY] = tyy; this->v_[YZ] = tyz;
                                          this->v_[ZZ] = tzz;
}


template<class Cmpt>
inline SymmTensor<Cmpt>::SymmTensor(Istream& is)
:
  VectorSpace<SymmTensor<Cmpt>, Cmpt, 6>{is}
{}


// Member Functions
template<class Cmpt>
inline const Cmpt&  SymmTensor<Cmpt>::xx() const
{
  return this->v_[XX];
}


template<class Cmpt>
inline const Cmpt&  SymmTensor<Cmpt>::xy() const
{
  return this->v_[XY];
}


template<class Cmpt>
inline const Cmpt&  SymmTensor<Cmpt>::xz() const
{
  return this->v_[XZ];
}


template<class Cmpt>
inline const Cmpt&  SymmTensor<Cmpt>::yy() const
{
  return this->v_[YY];
}


template<class Cmpt>
inline const Cmpt&  SymmTensor<Cmpt>::yz() const
{
  return this->v_[YZ];
}


template<class Cmpt>
inline const Cmpt&  SymmTensor<Cmpt>::zz() const
{
  return this->v_[ZZ];
}


template<class Cmpt>
inline Cmpt& SymmTensor<Cmpt>::xx()
{
  return this->v_[XX];
}


template<class Cmpt>
inline Cmpt& SymmTensor<Cmpt>::xy()
{
  return this->v_[XY];
}


template<class Cmpt>
inline Cmpt& SymmTensor<Cmpt>::xz()
{
  return this->v_[XZ];
}


template<class Cmpt>
inline Cmpt& SymmTensor<Cmpt>::yy()
{
  return this->v_[YY];
}


template<class Cmpt>
inline Cmpt& SymmTensor<Cmpt>::yz()
{
  return this->v_[YZ];
}


template<class Cmpt>
inline Cmpt& SymmTensor<Cmpt>::zz()
{
  return this->v_[ZZ];
}


template<class Cmpt>
inline const SymmTensor<Cmpt>& SymmTensor<Cmpt>::T() const
{
  return *this;
}


// Member Functions
template<class Cmpt>
inline void SymmTensor<Cmpt>::operator=(const SphericalTensor<Cmpt>& st)
{
  this->v_[XX] = st.ii(); this->v_[XY] = 0;       this->v_[XZ] = 0;
                          this->v_[YY] = st.ii(); this->v_[YZ] = 0;
                                                  this->v_[ZZ] = st.ii();
}


// Global Operators
//- Hodge Dual operator (tensor -> vector)
template<class Cmpt>
inline Vector<Cmpt> operator*(const SymmTensor<Cmpt>& st)
{
  return {st.yz(), -st.xz(), st.xy()};
}


//- Inner-product between two symmetric tensors
template<class Cmpt>
inline Tensor<Cmpt>
operator&(const SymmTensor<Cmpt>& st1, const SymmTensor<Cmpt>& st2)
{
  return {st1.xx()*st2.xx() + st1.xy()*st2.xy() + st1.xz()*st2.xz(),
          st1.xx()*st2.xy() + st1.xy()*st2.yy() + st1.xz()*st2.yz(),
          st1.xx()*st2.xz() + st1.xy()*st2.yz() + st1.xz()*st2.zz(),
          st1.xy()*st2.xx() + st1.yy()*st2.xy() + st1.yz()*st2.xz(),
          st1.xy()*st2.xy() + st1.yy()*st2.yy() + st1.yz()*st2.yz(),
          st1.xy()*st2.xz() + st1.yy()*st2.yz() + st1.yz()*st2.zz(),
          st1.xz()*st2.xx() + st1.yz()*st2.xy() + st1.zz()*st2.xz(),
          st1.xz()*st2.xy() + st1.yz()*st2.yy() + st1.zz()*st2.yz(),
          st1.xz()*st2.xz() + st1.yz()*st2.yz() + st1.zz()*st2.zz()};
}


//- Double-dot-product between a symmetric tensor and a symmetric tensor
template<class Cmpt>
inline Cmpt
operator&&(const SymmTensor<Cmpt>& st1, const SymmTensor<Cmpt>& st2)
{
  return st1.xx()*st2.xx() + 2*st1.xy()*st2.xy() + 2*st1.xz()*st2.xz()
    + st1.yy()*st2.yy() + 2*st1.yz()*st2.yz() + st1.zz()*st2.zz();
}


//- Inner-product between a symmetric tensor and a vector
template<class Cmpt>
inline Vector<Cmpt>
operator&(const SymmTensor<Cmpt>& st, const Vector<Cmpt>& v)
{
  return {st.xx()*v.x() + st.xy()*v.y() + st.xz()*v.z(),
          st.xy()*v.x() + st.yy()*v.y() + st.yz()*v.z(),
          st.xz()*v.x() + st.yz()*v.y() + st.zz()*v.z()};
}


//- Inner-product between a vector and a symmetric tensor
template<class Cmpt>
inline Vector<Cmpt>
operator&(const Vector<Cmpt>& v, const SymmTensor<Cmpt>& st)
{
  return {v.x()*st.xx() + v.y()*st.xy() + v.z()*st.xz(),
          v.x()*st.xy() + v.y()*st.yy() + v.z()*st.yz(),
          v.x()*st.xz() + v.y()*st.yz() + v.z()*st.zz()};
}


//- Inner-sqr of a symmetric tensor
template<class Cmpt>
inline SymmTensor<Cmpt>
innerSqr(const SymmTensor<Cmpt>& st)
{
  return {st.xx()*st.xx() + st.xy()*st.xy() + st.xz()*st.xz(),
          st.xx()*st.xy() + st.xy()*st.yy() + st.xz()*st.yz(),
          st.xx()*st.xz() + st.xy()*st.yz() + st.xz()*st.zz(),
          st.xy()*st.xy() + st.yy()*st.yy() + st.yz()*st.yz(),
          st.xy()*st.xz() + st.yy()*st.yz() + st.yz()*st.zz(),
          st.xz()*st.xz() + st.yz()*st.yz() + st.zz()*st.zz()};
}


template<class Cmpt>
inline Cmpt magSqr(const SymmTensor<Cmpt>& st)
{
  return magSqr(st.xx()) + 2*magSqr(st.xy()) + 2*magSqr(st.xz())
    + magSqr(st.yy()) + 2*magSqr(st.yz()) + magSqr(st.zz());
}


//- Return the trace of a symmetric tensor
template<class Cmpt>
inline Cmpt tr(const SymmTensor<Cmpt>& st)
{
  return st.xx() + st.yy() + st.zz();
}


//- Return the spherical part of a symmetric tensor
template<class Cmpt>
inline SphericalTensor<Cmpt> sph(const SymmTensor<Cmpt>& st)
{
  return (1.0/3.0)*tr(st);
}


//- Return the symmetric part of a symmetric tensor, i.e. itself
template<class Cmpt>
inline const SymmTensor<Cmpt>& symm(const SymmTensor<Cmpt>& st)
{
  return st;
}


//- Return twice the symmetric part of a symmetric tensor
template<class Cmpt>
inline SymmTensor<Cmpt> twoSymm(const SymmTensor<Cmpt>& st)
{
  return 2*st;
}


//- Return the deviatoric part of a symmetric tensor
template<class Cmpt>
inline SymmTensor<Cmpt> dev(const SymmTensor<Cmpt>& st)
{
  return st - SphericalTensor<Cmpt>::oneThirdI*tr(st);
}


//- Return the deviatoric part of a symmetric tensor
template<class Cmpt>
inline SymmTensor<Cmpt> dev2(const SymmTensor<Cmpt>& st)
{
  return st - SphericalTensor<Cmpt>::twoThirdsI*tr(st);
}


//- Return the determinant of a symmetric tensor
template<class Cmpt>
inline Cmpt det(const SymmTensor<Cmpt>& st)
{
  return st.xx()*st.yy()*st.zz() + st.xy()*st.yz()*st.xz() 
    + st.xz()*st.xy()*st.yz() - st.xx()*st.yz()*st.yz()
    - st.xy()*st.xy()*st.zz() - st.xz()*st.yy()*st.xz();
}


//- Return the cofactor symmetric tensor of a symmetric tensor
template<class Cmpt>
inline SymmTensor<Cmpt> cof(const SymmTensor<Cmpt>& st)
{
  return {st.yy()*st.zz() - st.yz()*st.yz(),
          st.xz()*st.yz() - st.xy()*st.zz(),
          st.xy()*st.yz() - st.xz()*st.yy(),
          st.xx()*st.zz() - st.xz()*st.xz(),
          st.xy()*st.xz() - st.xx()*st.yz(),
          st.xx()*st.yy() - st.xy()*st.xy()};
}


//- Return the inverse of a symmetric tensor give the determinant
template<class Cmpt>
inline SymmTensor<Cmpt> inv(const SymmTensor<Cmpt>& st, const Cmpt detst)
{
  return SymmTensor<Cmpt>(
    st.yy()*st.zz() - st.yz()*st.yz(),
    st.xz()*st.yz() - st.xy()*st.zz(),
    st.xy()*st.yz() - st.xz()*st.yy(),
    st.xx()*st.zz() - st.xz()*st.xz(),
    st.xy()*st.xz() - st.xx()*st.yz(),
    st.xx()*st.yy() - st.xy()*st.xy())/detst;
}


//- Return the inverse of a symmetric tensor
template<class Cmpt>
inline SymmTensor<Cmpt> inv(const SymmTensor<Cmpt>& st)
{
  return inv(st, det(st));
}


//- Return the 1st invariant of a symmetric tensor
template<class Cmpt>
inline Cmpt invariantI(const SymmTensor<Cmpt>& st) {
  return tr(st);
}
//- Return the 2nd invariant of a symmetric tensor
template<class Cmpt>
inline Cmpt invariantII(const SymmTensor<Cmpt>& st) {
  return 0.5*sqr(tr(st))
    - 0.5*(st.xx()*st.xx() + st.xy()*st.xy() + st.xz()*st.xz()
           + st.xy()*st.xy() + st.yy()*st.yy() + st.yz()*st.yz()
           + st.xz()*st.xz() + st.yz()*st.yz() + st.zz()*st.zz());
}
//- Return the 3rd invariant of a symmetric tensor
template<class Cmpt>
inline Cmpt invariantIII(const SymmTensor<Cmpt>& st)
{
  return det(st);
}


template<class Cmpt>
inline SymmTensor<Cmpt>
operator+(const SphericalTensor<Cmpt>& spt1, const SymmTensor<Cmpt>& st2)
{
  return {spt1.ii() + st2.xx(), st2.xy(), st2.xz(), spt1.ii() + st2.yy(),
    st2.yz(), spt1.ii() + st2.zz()};
}


template<class Cmpt>
inline SymmTensor<Cmpt>
operator+(const SymmTensor<Cmpt>& st1, const SphericalTensor<Cmpt>& spt2)
{
  return {st1.xx() + spt2.ii(), st1.xy(), st1.xz(), st1.yy() + spt2.ii(),
    st1.yz(), st1.zz() + spt2.ii()};
}


template<class Cmpt>
inline SymmTensor<Cmpt>
operator-(const SphericalTensor<Cmpt>& spt1, const SymmTensor<Cmpt>& st2)
{
  return {spt1.ii() - st2.xx(), -st2.xy(), -st2.xz(), spt1.ii() - st2.yy(),
    -st2.yz(), spt1.ii() - st2.zz()};
}


template<class Cmpt>
inline SymmTensor<Cmpt>
operator-(const SymmTensor<Cmpt>& st1, const SphericalTensor<Cmpt>& spt2)
{
  return {st1.xx() - spt2.ii(), st1.xy(), st1.xz(), st1.yy() - spt2.ii(),
    st1.yz(), st1.zz() - spt2.ii()};
}


//- Inner-product between a spherical symmetric tensor and a symmetric tensor
template<class Cmpt>
inline SymmTensor<Cmpt>
operator&(const SphericalTensor<Cmpt>& spt1, const SymmTensor<Cmpt>& st2)
{
  return {spt1.ii()*st2.xx(), spt1.ii()*st2.xy(), spt1.ii()*st2.xz(),
    spt1.ii()*st2.yy(), spt1.ii()*st2.yz(), spt1.ii()*st2.zz()};
}


//- Inner-product between a tensor and a spherical tensor
template<class Cmpt>
inline SymmTensor<Cmpt>
operator&(const SymmTensor<Cmpt>& st1, const SphericalTensor<Cmpt>& spt2)
{
  return {st1.xx()*spt2.ii(), st1.xy()*spt2.ii(), st1.xz()*spt2.ii(),
    st1.yy()*spt2.ii(), st1.yz()*spt2.ii(), st1.zz()*spt2.ii()};
}


//- Double-dot-product between a spherical tensor and a symmetric tensor
template<class Cmpt>
inline Cmpt
operator&&(const SphericalTensor<Cmpt>& spt1, const SymmTensor<Cmpt>& st2)
{
  return spt1.ii()*st2.xx() + spt1.ii()*st2.yy() + spt1.ii()*st2.zz();
}


//- Double-dot-product between a tensor and a spherical tensor
template<class Cmpt>
inline Cmpt
operator&&(const SymmTensor<Cmpt>& st1, const SphericalTensor<Cmpt>& spt2)
{
  return st1.xx()*spt2.ii() + st1.yy()*spt2.ii() + st1.zz()*spt2.ii();
}


template<class Cmpt>
inline SymmTensor<Cmpt> sqr(const Vector<Cmpt>& v)
{
  return {v.x()*v.x(), v.x()*v.y(), v.x()*v.z(), v.y()*v.y(), v.y()*v.z(),
    v.z()*v.z()};
}


template<class Cmpt>
class outerProduct<SymmTensor<Cmpt>, Cmpt>
{
public:
  typedef SymmTensor<Cmpt> type;
};


template<class Cmpt>
class outerProduct<Cmpt, SymmTensor<Cmpt>>
{
public:
  typedef SymmTensor<Cmpt> type;
};


template<class Cmpt>
class innerProduct<SymmTensor<Cmpt>, SymmTensor<Cmpt>>
{
public:
  typedef Tensor<Cmpt> type;
};


template<class Cmpt>
class innerProduct<SymmTensor<Cmpt>, Vector<Cmpt>>
{
public:
  typedef Vector<Cmpt> type;
};


template<class Cmpt>
class innerProduct<Vector<Cmpt>, SymmTensor<Cmpt>>
{
public:
  typedef Vector<Cmpt> type;
};


template<class Cmpt>
class typeOfSum<SphericalTensor<Cmpt>, SymmTensor<Cmpt>>
{
public:
  typedef SymmTensor<Cmpt> type;
};


template<class Cmpt>
class typeOfSum<SymmTensor<Cmpt>, SphericalTensor<Cmpt>>
{
public:
  typedef SymmTensor<Cmpt> type;
};


template<class Cmpt>
class innerProduct<SphericalTensor<Cmpt>, SymmTensor<Cmpt>>
{
public:
  typedef SymmTensor<Cmpt> type;
};


template<class Cmpt>
class innerProduct<SymmTensor<Cmpt>, SphericalTensor<Cmpt>>
{
public:
  typedef SymmTensor<Cmpt> type;
};

}  // namespace mousse

#endif
