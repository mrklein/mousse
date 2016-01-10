// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ICCG
// Description
//   Incomplete Cholesky preconditioned CG solver derived from the general
//   preconditioned CG solver PCG but with the choice of preconditioner
//   pre-selected.
// Deprecated
//   This solver is present for backward-compatibility and the PCG solver
//   should be used for preference. (deprecated Apr 2008)
// SourceFiles
//   iccg.cpp

#ifndef iccg_hpp_
#define iccg_hpp_

#include "pcg.hpp"

namespace mousse
{

class ICCG
:
  public PCG
{
public:
    //- Return the dictionary constructed from the components.
    //  Needed for backward compatibility
    static dictionary solverDict
    (
      const scalar tol,
      const scalar relTol
    );

    //- Return the dictionary constructed from the old-style data-stream.
    //  Needed for backward compatibility
    static dictionary solverDict(Istream&);

  //- Runtime type information
  TYPE_NAME("ICCG");

  // Constructors

    //- Construct from matrix components and solver data stream
    ICCG
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const dictionary& solverControls
    );

    //- Construct from matrix components and tolerances
    ICCG
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const scalar tolerance,
      const scalar relTol = 0.0
    );

    //- Disallow default bitwise copy construct
    ICCG(const ICCG&) = delete;

    //- Disallow default bitwise assignment
    ICCG& operator=(const ICCG&) = delete;

  //- Destructor
  virtual ~ICCG()
  {}

};

}  // namespace mousse
#endif
