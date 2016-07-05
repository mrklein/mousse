// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "simple_matrix.hpp"


// Constructors 
template<class Type>
mousse::simpleMatrix<Type>::simpleMatrix(const label mSize)
:
  scalarSquareMatrix{mSize},
  source_{mSize}
{}


template<class Type>
mousse::simpleMatrix<Type>::simpleMatrix
(
  const label mSize,
  const scalar coeffVal,
  const Type& sourceVal
)
:
  scalarSquareMatrix{mSize, mSize, coeffVal},
  source_{mSize, sourceVal}
{}


template<class Type>
mousse::simpleMatrix<Type>::simpleMatrix
(
  const scalarSquareMatrix& matrix,
  const Field<Type>& source
)
:
  scalarSquareMatrix{matrix},
  source_{source}
{}


template<class Type>
mousse::simpleMatrix<Type>::simpleMatrix(Istream& is)
:
  scalarSquareMatrix{is},
  source_{is}
{}


// Member Functions 
template<class Type>
mousse::Field<Type> mousse::simpleMatrix<Type>::solve() const
{
  scalarSquareMatrix tmpMatrix = *this;
  Field<Type> sourceSol = source_;
  mousse::solve(tmpMatrix, sourceSol);
  return sourceSol;
}


template<class Type>
mousse::Field<Type> mousse::simpleMatrix<Type>::LUsolve() const
{
  scalarSquareMatrix luMatrix = *this;
  Field<Type> sourceSol = source_;
  mousse::LUsolve(luMatrix, sourceSol);
  return sourceSol;
}


// Member Operators 
template<class Type>
void mousse::simpleMatrix<Type>::operator=(const simpleMatrix<Type>& m)
{
  if (this == &m) {
    FATAL_ERROR_IN("simpleMatrix<Type>::operator=(const simpleMatrix<Type>&)")
      << "Attempted assignment to self"
      << abort(FatalError);
  }
  if (n() != m.n()) {
    FATAL_ERROR_IN("simpleMatrix<Type>::operator=(const simpleMatrix<Type>&)")
      << "Different size matrices"
      << abort(FatalError);
  }
  if (source_.size() != m.source_.size()) {
    FATAL_ERROR_IN("simpleMatrix<Type>::operator=(const simpleMatrix<Type>&)")
      << "Different size source vectors"
      << abort(FatalError);
  }
  scalarSquareMatrix::operator=(m);
  source_ = m.source_;
}


// Friend Operators 
template<class Type>
mousse::simpleMatrix<Type> mousse::operator+
(
  const simpleMatrix<Type>& m1,
  const simpleMatrix<Type>& m2
)
{
  return // simpleMatrix<Type>
  {
    static_cast<const scalarSquareMatrix&>(m1)
      + static_cast<const scalarSquareMatrix&>(m2),
    m1.source_ + m2.source_
  };
}


template<class Type>
mousse::simpleMatrix<Type> mousse::operator-
(
  const simpleMatrix<Type>& m1,
  const simpleMatrix<Type>& m2
)
{
  return // simpleMatrix<Type>
  {
    static_cast<const scalarSquareMatrix&>(m1)
      - static_cast<const scalarSquareMatrix&>(m2),
    m1.source_ - m2.source_
  };
}


template<class Type>
mousse::simpleMatrix<Type> mousse::operator*
(
  const scalar s,
  const simpleMatrix<Type>& m
)
{
  // return simpleMatrix<Type>{s*m.matrix_, s*m.source_};
  return {s*m.matrix_, s*m.source_};
}


// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const simpleMatrix<Type>& m
)
{
  os << static_cast<const scalarSquareMatrix&>(m) << nl << m.source_;
  return os;
}
