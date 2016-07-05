// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"


void mousse::lduMatrix::sumDiag()
{
  const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
  const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();
  scalarField& Diag = diag();
  const labelUList& l = lduAddr().lowerAddr();
  const labelUList& u = lduAddr().upperAddr();
  for (label face=0; face<l.size(); face++) {
    Diag[l[face]] += Lower[face];
    Diag[u[face]] += Upper[face];
  }
}


void mousse::lduMatrix::negSumDiag()
{
  const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
  const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();
  scalarField& Diag = diag();
  const labelUList& l = lduAddr().lowerAddr();
  const labelUList& u = lduAddr().upperAddr();
  for (label face=0; face<l.size(); face++) {
    Diag[l[face]] -= Lower[face];
    Diag[u[face]] -= Upper[face];
  }
}


void mousse::lduMatrix::sumMagOffDiag
(
  scalarField& sumOff
) const
{
  const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
  const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();
  const labelUList& l = lduAddr().lowerAddr();
  const labelUList& u = lduAddr().upperAddr();
  for (label face = 0; face < l.size(); face++) {
    sumOff[u[face]] += mag(Lower[face]);
    sumOff[l[face]] += mag(Upper[face]);
  }
}


void mousse::lduMatrix::operator=(const lduMatrix& A)
{
  if (this == &A) {
    FatalError
      << "lduMatrix::operator=(const lduMatrix&) : "
      << "attempted assignment to self"
      << abort(FatalError);
  }
  if (A.lowerPtr_ != nullptr) {
    lower() = A.lower();
  } else if (lowerPtr_ != nullptr) {
    delete lowerPtr_;
    lowerPtr_ = nullptr;
  }
  if (A.upperPtr_ != nullptr) {
    upper() = A.upper();
  } else if (upperPtr_ != nullptr) {
    delete upperPtr_;
    upperPtr_ = nullptr;
  }
  if (A.diagPtr_ != nullptr) {
    diag() = A.diag();
  }
}


void mousse::lduMatrix::negate()
{
  if (lowerPtr_ != nullptr) {
    lowerPtr_->negate();
  }
  if (upperPtr_ != nullptr) {
    upperPtr_->negate();
  }
  if (diagPtr_ != nullptr) {
    diagPtr_->negate();
  }
}


void mousse::lduMatrix::operator+=(const lduMatrix& A)
{
  if (A.diagPtr_ != nullptr) {
    diag() += A.diag();
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
  } else {
    if (debug > 1) {
      WARNING_IN("lduMatrix::operator+=(const lduMatrix& A)")
        << "Unknown matrix type combination" << nl
        << "    this :"
        << " diagonal:" << diagonal()
        << " symmetric:" << symmetric()
        << " asymmetric:" << asymmetric() << nl
        << "    A    :"
        << " diagonal:" << A.diagonal()
        << " symmetric:" << A.symmetric()
        << " asymmetric:" << A.asymmetric()
        << endl;
    }
  }
}


void mousse::lduMatrix::operator-=(const lduMatrix& A)
{
  if (A.diagPtr_ != nullptr) {
    diag() -= A.diag();
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
    if (debug > 1) {
      WARNING_IN("lduMatrix::operator-=(const lduMatrix& A)")
        << "Unknown matrix type combination" << nl
        << "    this :"
        << " diagonal:" << diagonal()
        << " symmetric:" << symmetric()
        << " asymmetric:" << asymmetric() << nl
        << "    A    :"
        << " diagonal:" << A.diagonal()
        << " symmetric:" << A.symmetric()
        << " asymmetric:" << A.asymmetric()
        << endl;
    }
  }
}


void mousse::lduMatrix::operator*=(const scalarField& sf)
{
  if (diagPtr_ != nullptr) {
    *diagPtr_ *= sf;
  }
  // Non-uniform scaling causes a symmetric matrix
  // to become asymmetric
  if (symmetric() || asymmetric()) {
    scalarField& upper = this->upper();
    scalarField& lower = this->lower();
    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();
    for (label face=0; face<upper.size(); face++) {
      upper[face] *= sf[l[face]];
    }
    for (label face=0; face<lower.size(); face++) {
      lower[face] *= sf[u[face]];
    }
  }
}


void mousse::lduMatrix::operator*=(scalar s)
{
  if (diagPtr_ != nullptr) {
    *diagPtr_ *= s;
  }
  if (upperPtr_ != nullptr) {
    *upperPtr_ *= s;
  }
  if (lowerPtr_ != nullptr) {
    *lowerPtr_ *= s;
  }
}
