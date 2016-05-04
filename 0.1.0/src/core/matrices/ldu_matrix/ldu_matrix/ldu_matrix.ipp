// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"


template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::lduMatrix::H(const Field<Type>& psi) const
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
    const scalar* __restrict__ lowerPtr = lower().begin();
    const scalar* __restrict__ upperPtr = upper().begin();
    const label nFaces = upper().size();
    for (label face=0; face<nFaces; face++) {
      HpsiPtr[uPtr[face]] -= lowerPtr[face]*psiPtr[lPtr[face]];
      HpsiPtr[lPtr[face]] -= upperPtr[face]*psiPtr[uPtr[face]];
    }
  }
  return tHpsi;
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::lduMatrix::H(const tmp<Field<Type> >& tpsi) const
{
  tmp<Field<Type>> tHpsi{H(tpsi())};
  tpsi.clear();
  return tHpsi;
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::lduMatrix::faceH(const Field<Type>& psi) const
{
  if (lowerPtr_ != nullptr || upperPtr_ != nullptr) {
    const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
    const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();
    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();
    tmp<Field<Type>> tfaceHpsi{new Field<Type>{Lower.size()}};
    Field<Type> & faceHpsi = tfaceHpsi();
    for (label face=0; face<l.size(); face++) {
      faceHpsi[face] = Upper[face]*psi[u[face]] - Lower[face]*psi[l[face]];
    }
    return tfaceHpsi;
  } else {
    FATAL_ERROR_IN("lduMatrix::faceH(const Field<Type>& psi) const")
      << "Cannot calculate faceH"
         " the matrix does not have any off-diagonal coefficients."
      << exit(FatalError);

    return tmp<Field<Type>>{nullptr};
  }
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::lduMatrix::faceH(const tmp<Field<Type> >& tpsi) const
{
  tmp<Field<Type>> tfaceHpsi{faceH(tpsi())};
  tpsi.clear();
  return tfaceHpsi;
}
