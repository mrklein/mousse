// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::sumDiag()
{
  const Field<LUType>& Lower = const_cast<const LduMatrix&>(*this).lower();
  const Field<LUType>& Upper = const_cast<const LduMatrix&>(*this).upper();
  Field<DType>& Diag = diag();
  const unallocLabelList& l = lduAddr().lowerAddr();
  const unallocLabelList& u = lduAddr().upperAddr();
  for (label face=0; face<l.size(); face++) {
    Diag[l[face]] += Lower[face];
    Diag[u[face]] += Upper[face];
  }
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::negSumDiag()
{
  const Field<LUType>& Lower = const_cast<const LduMatrix&>(*this).lower();
  const Field<LUType>& Upper = const_cast<const LduMatrix&>(*this).upper();
  Field<DType>& Diag = diag();
  const unallocLabelList& l = lduAddr().lowerAddr();
  const unallocLabelList& u = lduAddr().upperAddr();
  for (label face=0; face<l.size(); face++) {
    Diag[l[face]] -= Lower[face];
    Diag[u[face]] -= Upper[face];
  }
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::sumMagOffDiag
(
  Field<LUType>& sumOff
) const
{
  const Field<LUType>& Lower = const_cast<const LduMatrix&>(*this).lower();
  const Field<LUType>& Upper = const_cast<const LduMatrix&>(*this).upper();
  const unallocLabelList& l = lduAddr().lowerAddr();
  const unallocLabelList& u = lduAddr().upperAddr();
  for (label face = 0; face < l.size(); face++) {
    sumOff[u[face]] += cmptMag(Lower[face]);
    sumOff[l[face]] += cmptMag(Upper[face]);
  }
}


template<class Type, class DType, class LUType>
mousse::tmp<mousse::Field<Type>>
mousse::LduMatrix<Type, DType, LUType>::H(const Field<Type>& psi) const
{
  tmp<Field<Type>> tHpsi
  {
    new Field<Type>{lduAddr().size(), pTraits<Type>::zero}
  };
  if (lowerPtr_ != nullptr || upperPtr_ != nullptr) {
    Field<Type> & Hpsi = tHpsi();
    Type* __restrict__ HpsiPtr = Hpsi.begin();
    const Type* __restrict__ psiPtr = psi.begin();
    const label* __restrict__ uPtr = lduAddr().upperAddr().begin();
    const label* __restrict__ lPtr = lduAddr().lowerAddr().begin();
    const LUType* __restrict__ lowerPtr = lower().begin();
    const LUType* __restrict__ upperPtr = upper().begin();
    const label nFaces = upper().size();
    for (label face=0; face<nFaces; face++) {
      HpsiPtr[uPtr[face]] -= lowerPtr[face]*psiPtr[lPtr[face]];
      HpsiPtr[lPtr[face]] -= upperPtr[face]*psiPtr[uPtr[face]];
    }
  }
  return tHpsi;
}


template<class Type, class DType, class LUType>
mousse::tmp<mousse::Field<Type>>
mousse::LduMatrix<Type, DType, LUType>::H(const tmp<Field<Type>>& tpsi) const
{
  tmp<Field<Type>> tHpsi{H(tpsi())};
  tpsi.clear();
  return tHpsi;
}


template<class Type, class DType, class LUType>
mousse::tmp<mousse::Field<Type>>
mousse::LduMatrix<Type, DType, LUType>::faceH(const Field<Type>& psi) const
{
  const Field<LUType>& Lower = const_cast<const LduMatrix&>(*this).lower();
  const Field<LUType>& Upper = const_cast<const LduMatrix&>(*this).upper();
  // Take refereces to addressing
  const unallocLabelList& l = lduAddr().lowerAddr();
  const unallocLabelList& u = lduAddr().upperAddr();
  tmp<Field<Type>> tfaceHpsi{new Field<Type>{Lower.size()}};
  Field<Type> & faceHpsi = tfaceHpsi();
  for (label face=0; face<l.size(); face++) {
    faceHpsi[face] = Upper[face]*psi[u[face]] - Lower[face]*psi[l[face]];
  }
  return tfaceHpsi;
}


template<class Type, class DType, class LUType>
mousse::tmp<mousse::Field<Type>>
mousse::LduMatrix<Type, DType, LUType>::faceH(const tmp<Field<Type>>& tpsi) const
{
  tmp<Field<Type>> tfaceHpsi{faceH(tpsi())};
  tpsi.clear();
  return tfaceHpsi;
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::operator=(const LduMatrix& A)
{
  if (this == &A) {
    FATAL_ERROR_IN
    (
      "LduMatrix<Type, DType, LUType>::operator=(const LduMatrix&)"
    )
    << "attempted assignment to self"
    << abort(FatalError);
  }
  if (A.diagPtr_ != nullptr) {
    diag() = A.diag();
  }
  if (A.upperPtr_ != nullptr) {
    upper() = A.upper();
  } else if (upperPtr_ != nullptr) {
    delete upperPtr_;
    upperPtr_ = nullptr;
  }
  if (A.lowerPtr_ != nullptr) {
    lower() = A.lower();
  } else if (lowerPtr_ != nullptr) {
    delete lowerPtr_;
    lowerPtr_ = nullptr;
  }
  if (A.sourcePtr_ != nullptr) {
    source() = A.source();
  }
  interfacesUpper_ = A.interfacesUpper_;
  interfacesLower_ = A.interfacesLower_;
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::negate()
{
  if (diagPtr_ != nullptr) {
    diagPtr_->negate();
  }
  if (upperPtr_ != nullptr) {
    upperPtr_->negate();
  }
  if (lowerPtr_ != nullptr) {
    lowerPtr_->negate();
  }
  if (sourcePtr_ != nullptr) {
    sourcePtr_->negate();
  }
  negate(interfacesUpper_);
  negate(interfacesLower_);
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::operator+=(const LduMatrix& A)
{
  if (A.diagPtr_ != nullptr) {
    diag() += A.diag();
  }
  if (A.sourcePtr_ != nullptr) {
    source() += A.source();
  }
  if (symmetric() && A.symmetric()) {
    upper() += A.upper();
  } else if (symmetric() && A.asymmetric()) {
    if (upperPtr_ != nullptr) {
      lower();
    } else {
      upper();
    }
    upper() += A.upper();
    lower() += A.lower();
  } else if (asymmetric() && A.symmetric()) {
    if (A.upperPtr_ != nullptr) {
      lower() += A.upper();
      upper() += A.upper();
    } else {
      lower() += A.lower();
      upper() += A.lower();
    }
  } else if (asymmetric() && A.asymmetric()) {
    lower() += A.lower();
    upper() += A.upper();
  } else if (diagonal()) {
    if (A.upperPtr_ != nullptr) {
      upper() = A.upper();
    }
    if (A.lowerPtr_ != nullptr) {
      lower() = A.lower();
    }
  } else if (A.diagonal()) {
  } else
  {
    FATAL_ERROR_IN
    (
      "LduMatrix<Type, DType, LUType>::operator+=(const LduMatrix& A)"
    )
    << "Unknown matrix type combination"
    << abort(FatalError);
  }
  interfacesUpper_ += A.interfacesUpper_;
  interfacesLower_ += A.interfacesLower_;
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::operator-=(const LduMatrix& A)
{
  if (A.diagPtr_ != nullptr) {
    diag() -= A.diag();
  }
  if (A.sourcePtr_ != nullptr) {
    source() -= A.source();
  }
  if (symmetric() && A.symmetric()) {
    upper() -= A.upper();
  } else if (symmetric() && A.asymmetric()) {
    if (upperPtr_ != nullptr) {
      lower();
    } else {
      upper();
    }
    upper() -= A.upper();
    lower() -= A.lower();
  } else if (asymmetric() && A.symmetric()) {
    if (A.upperPtr_ != nullptr) {
      lower() -= A.upper();
      upper() -= A.upper();
    } else {
      lower() -= A.lower();
      upper() -= A.lower();
    }
  } else if (asymmetric() && A.asymmetric()) {
    lower() -= A.lower();
    upper() -= A.upper();
  } else if (diagonal()) {
    if (A.upperPtr_ != nullptr) {
      upper() = -A.upper();
    }
    if (A.lowerPtr_ != nullptr) {
      lower() = -A.lower();
    }
  } else if (A.diagonal()) {
  } else {
    FATAL_ERROR_IN
    (
      "LduMatrix<Type, DType, LUType>::operator-=(const LduMatrix& A)"
    )
    << "Unknown matrix type combination"
    << abort(FatalError);
  }
  interfacesUpper_ -= A.interfacesUpper_;
  interfacesLower_ -= A.interfacesLower_;
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::operator*=
(
  const scalarField& sf
)
{
  if (diagPtr_ != nullptr) {
    *diagPtr_ *= sf;
  }
  if (sourcePtr_ != nullptr) {
    *sourcePtr_ *= sf;
  }
  // Non-uniform scaling causes a symmetric matrix
  // to become asymmetric
  if (symmetric() || asymmetric()) {
    Field<LUType>& upper = this->upper();
    Field<LUType>& lower = this->lower();
    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();
    for (label face=0; face < upper.size(); face++) {
      upper[face] *= sf[l[face]];
    }
    for (label face=0; face < lower.size(); face++) {
      lower[face] *= sf[u[face]];
    }
  }
  FATAL_ERROR_IN
  (
    "LduMatrix<Type, DType, LUType>::operator*=(const scalarField& sf)"
  )
  << "Scaling a matrix by scalarField is not currently supported\n"
  "because scaling interfacesUpper_ and interfacesLower_ "
  "require special transfers"
  << abort(FatalError);
}


template<class Type, class DType, class LUType>
void mousse::LduMatrix<Type, DType, LUType>::operator*=(scalar s)
{
  if (diagPtr_ != nullptr) {
    *diagPtr_ *= s;
  }
  if (sourcePtr_ != nullptr) {
    *sourcePtr_ *= s;
  }
  if (upperPtr_ != nullptr) {
    *upperPtr_ *= s;
  }
  if (lowerPtr_ != nullptr) {
    *lowerPtr_ *= s;
  }
  interfacesUpper_ *= s;
  interfacesLower_ *= s;
}

