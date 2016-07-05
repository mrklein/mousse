// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_matrix.hpp"


// Static Data Members
namespace mousse {

DEFINE_RUN_TIME_SELECTION_TABLE(lduMatrix::smoother, symMatrix);
DEFINE_RUN_TIME_SELECTION_TABLE(lduMatrix::smoother, asymMatrix);

}

mousse::word
mousse::lduMatrix::smoother::getName
(
  const dictionary& solverControls
)
{
  word name;
  // handle primitive or dictionary entry
  const entry& e = solverControls.lookupEntry("smoother", false, false);
  if (e.isDict()) {
    e.dict().lookup("smoother") >> name;
  } else {
    e.stream() >> name;
  }
  return name;
}


mousse::autoPtr<mousse::lduMatrix::smoother> mousse::lduMatrix::smoother::New
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  const dictionary& solverControls
)
{
  word name;
  // handle primitive or dictionary entry
  const entry& e = solverControls.lookupEntry("smoother", false, false);
  if (e.isDict()) {
    e.dict().lookup("smoother") >> name;
  } else {
    e.stream() >> name;
  }
  // not (yet?) needed:
  // const dictionary& controls = e.isDict() ? e.dict() : dictionary::null;
  if (matrix.symmetric()) {
    symMatrixConstructorTable::iterator constructorIter =
      symMatrixConstructorTablePtr_->find(name);
    if (constructorIter == symMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "lduMatrix::smoother::New", solverControls
      )
      << "Unknown symmetric matrix smoother "
      << name << nl << nl
      << "Valid symmetric matrix smoothers are :" << endl
      << symMatrixConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
    }
    return
      autoPtr<lduMatrix::smoother>
      {
        constructorIter()
        (
          fieldName,
          matrix,
          interfaceBouCoeffs,
          interfaceIntCoeffs,
          interfaces
        )
      };
  } else if (matrix.asymmetric()) {
    asymMatrixConstructorTable::iterator constructorIter =
      asymMatrixConstructorTablePtr_->find(name);
    if (constructorIter == asymMatrixConstructorTablePtr_->end()) {
      FATAL_IO_ERROR_IN
      (
        "lduMatrix::smoother::New", solverControls
      )
      << "Unknown asymmetric matrix smoother "
      << name << nl << nl
      << "Valid asymmetric matrix smoothers are :" << endl
      << asymMatrixConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
    }
    return
      autoPtr<lduMatrix::smoother>
      {
        constructorIter()
        (
          fieldName,
          matrix,
          interfaceBouCoeffs,
          interfaceIntCoeffs,
          interfaces
        )
      };
  } else {
    FATAL_IO_ERROR_IN
    (
      "lduMatrix::smoother::New", solverControls
    )
    << "cannot solve incomplete matrix, "
       "no diagonal or off-diagonal coefficient"
    << exit(FatalIOError);
    return autoPtr<lduMatrix::smoother>{NULL};
  }
}


// Constructors 
mousse::lduMatrix::smoother::smoother
(
  const word& fieldName,
  const lduMatrix& matrix,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces
)
:
  fieldName_{fieldName},
  matrix_{matrix},
  interfaceBouCoeffs_{interfaceBouCoeffs},
  interfaceIntCoeffs_{interfaceIntCoeffs},
  interfaces_{interfaces}
{}
