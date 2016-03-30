// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Type, class DType, class LUType>
mousse::LduMatrix<Type, DType, LUType>::LduMatrix(const lduMesh& mesh)
:
  lduMesh_{mesh},
  diagPtr_{NULL},
  upperPtr_{NULL},
  lowerPtr_{NULL},
  sourcePtr_{NULL},
  interfaces_{0},
  interfacesUpper_{0},
  interfacesLower_{0}
{}


template<class Type, class DType, class LUType>
mousse::LduMatrix<Type, DType, LUType>::LduMatrix(const LduMatrix& A)
:
  lduMesh_{A.lduMesh_},
  diagPtr_{NULL},
  upperPtr_{NULL},
  lowerPtr_{NULL},
  sourcePtr_{NULL},
  interfaces_{0},
  interfacesUpper_{0},
  interfacesLower_{0}
{
  if (A.diagPtr_) {
    diagPtr_ = new Field<DType>(*(A.diagPtr_));
  }
  if (A.upperPtr_) {
    upperPtr_ = new Field<LUType>(*(A.upperPtr_));
  }
  if (A.lowerPtr_) {
    lowerPtr_ = new Field<LUType>(*(A.lowerPtr_));
  }
  if (A.sourcePtr_) {
    sourcePtr_ = new Field<Type>(*(A.sourcePtr_));
  }
}


template<class Type, class DType, class LUType>
mousse::LduMatrix<Type, DType, LUType>::LduMatrix(LduMatrix& A, bool reUse)
:
  lduMesh_{A.lduMesh_},
  diagPtr_{NULL},
  upperPtr_{NULL},
  lowerPtr_{NULL},
  sourcePtr_{NULL},
  interfaces_{0},
  interfacesUpper_{0},
  interfacesLower_{0}
{
  if (reUse) {
    if (A.diagPtr_) {
      diagPtr_ = A.diagPtr_;
      A.diagPtr_ = NULL;
    }
    if (A.upperPtr_) {
      upperPtr_ = A.upperPtr_;
      A.upperPtr_ = NULL;
    }
    if (A.lowerPtr_) {
      lowerPtr_ = A.lowerPtr_;
      A.lowerPtr_ = NULL;
    }
    if (A.sourcePtr_) {
      sourcePtr_ = A.sourcePtr_;
      A.sourcePtr_ = NULL;
    }
  } else {
    if (A.diagPtr_) {
      diagPtr_ = new Field<DType>(*(A.diagPtr_));
    }
    if (A.upperPtr_) {
      upperPtr_ = new Field<LUType>(*(A.upperPtr_));
    }
    if (A.lowerPtr_) {
      lowerPtr_ = new Field<LUType>(*(A.lowerPtr_));
    }
    if (A.sourcePtr_) {
      sourcePtr_ = new Field<Type>(*(A.sourcePtr_));
    }
  }
}


template<class Type, class DType, class LUType>
mousse::LduMatrix<Type, DType, LUType>::LduMatrix
(
  const lduMesh& mesh,
  Istream& is
)
:
  lduMesh_{mesh},
  diagPtr_{new Field<DType>(is)},
  upperPtr_{new Field<LUType>(is)},
  lowerPtr_{new Field<LUType>(is)},
  sourcePtr_{new Field<Type>(is)},
  interfaces_{0},
  interfacesUpper_{0},
  interfacesLower_{0}
{}


// Destructor 
template<class Type, class DType, class LUType>
mousse::LduMatrix<Type, DType, LUType>::~LduMatrix()
{
  if (diagPtr_) {
    delete diagPtr_;
  }
  if (upperPtr_) {
    delete upperPtr_;
  }
  if (lowerPtr_) {
    delete lowerPtr_;
  }
  if (sourcePtr_) {
    delete sourcePtr_;
  }
}


// Member Functions 
template<class Type, class DType, class LUType>
mousse::Field<DType>& mousse::LduMatrix<Type, DType, LUType>::diag()
{
  if (!diagPtr_) {
    diagPtr_ = new Field<DType>{lduAddr().size(), pTraits<DType>::zero};
  }
  return *diagPtr_;
}


template<class Type, class DType, class LUType>
mousse::Field<LUType>& mousse::LduMatrix<Type, DType, LUType>::upper()
{
  if (!upperPtr_) {
    if (lowerPtr_) {
      upperPtr_ = new Field<LUType>{*lowerPtr_};
    } else {
      upperPtr_ = new Field<LUType>
      {
        lduAddr().lowerAddr().size(),
        pTraits<LUType>::zero
      };
    }
  }
  return *upperPtr_;
}


template<class Type, class DType, class LUType>
mousse::Field<LUType>& mousse::LduMatrix<Type, DType, LUType>::lower()
{
  if (!lowerPtr_) {
    if (upperPtr_) {
      lowerPtr_ = new Field<LUType>{*upperPtr_};
    } else {
      lowerPtr_ = new Field<LUType>
      {
        lduAddr().lowerAddr().size(),
        pTraits<LUType>::zero
      };
    }
  }
  return *lowerPtr_;
}


template<class Type, class DType, class LUType>
mousse::Field<Type>& mousse::LduMatrix<Type, DType, LUType>::source()
{
  if (!sourcePtr_) {
    sourcePtr_ = new Field<Type>{lduAddr().size(), pTraits<Type>::zero};
  }
  return *sourcePtr_;
}


template<class Type, class DType, class LUType>
const mousse::Field<DType>& mousse::LduMatrix<Type, DType, LUType>::diag() const
{
  if (!diagPtr_) {
    FATAL_ERROR_IN
    (
      "const Field<DType>& LduMatrix<Type, DType, LUType>::diag() const"
    )
    << "diagPtr_ unallocated"
    << abort(FatalError);
  }
  return *diagPtr_;
}


template<class Type, class DType, class LUType>
const mousse::Field<LUType>& mousse::LduMatrix<Type, DType, LUType>::upper() const
{
  if (!lowerPtr_ && !upperPtr_) {
    FATAL_ERROR_IN
    (
      "const Field<LUType>& LduMatrix<Type, DType, LUType>::upper() const"
    )
    << "lowerPtr_ or upperPtr_ unallocated"
    << abort(FatalError);
  }
  if (upperPtr_) {
    return *upperPtr_;
  } else {
    return *lowerPtr_;
  }
}


template<class Type, class DType, class LUType>
const mousse::Field<LUType>& mousse::LduMatrix<Type, DType, LUType>::lower() const
{
  if (!lowerPtr_ && !upperPtr_) {
    FATAL_ERROR_IN
    (
      "const Field<LUType>& LduMatrix<Type, DType, LUType>::lower() const"
    )
    << "lowerPtr_ or upperPtr_ unallocated"
    << abort(FatalError);
  }
  if (lowerPtr_) {
    return *lowerPtr_;
  } else {
    return *upperPtr_;
  }
}


template<class Type, class DType, class LUType>
const mousse::Field<Type>& mousse::LduMatrix<Type, DType, LUType>::source() const
{
  if (!sourcePtr_) {
    FATAL_ERROR_IN
    (
      "const Field<Type>& LduMatrix<Type, DType, LUType>::source() const"
    )
    << "sourcePtr_ unallocated"
    << abort(FatalError);
  }

  return *sourcePtr_;
}


// Friend Operators 
template<class Type, class DType, class LUType>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const LduMatrix<Type, DType, LUType>& ldum
)
{
  if (ldum.diagPtr_) {
    os << "Diagonal = "
      << *ldum.diagPtr_
      << endl << endl;
  }
  if (ldum.upperPtr_) {
    os << "Upper triangle = "
      << *ldum.upperPtr_
      << endl << endl;
  }
  if (ldum.lowerPtr_) {
    os << "Lower triangle = "
      << *ldum.lowerPtr_
      << endl << endl;
  }
  if (ldum.sourcePtr_) {
    os << "Source = "
      << *ldum.sourcePtr_
      << endl << endl;
  }
  os.check("Ostream& operator<<(Ostream&, const LduMatrix&");
  return os;
}

#include "_ldu_matrix_operations.ipp"
#include "_ldu_matrix_at_mul.ipp"
#include "_ldu_matrix_update_matrix_interfaces.ipp"
#include "_ldu_matrix_preconditioner.ipp"
#include "_ldu_matrix_smoother.ipp"
#include "_ldu_matrix_solver.ipp"
