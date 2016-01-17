// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PCG
// Description
//   Preconditioned conjugate gradient solver for symmetric lduMatrices
//   using a run-time selectable preconditioner.
// SourceFiles
//   pcg.cpp

#ifndef pcg_hpp_
#define pcg_hpp_

#include "ldu_matrix.hpp"

namespace mousse
{

class PCG
:
  public lduMatrix::solver
{
public:

  //- Runtime type information
  TYPE_NAME("PCG");

  // Constructors

    //- Construct from matrix components and solver controls
    PCG
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const dictionary& solverControls
    );

    //- Disallow default bitwise copy construct
    PCG(const PCG&) = delete;

    //- Disallow default bitwise assignment
    PCG& operator=(const PCG&) = delete;

  //- Destructor
  virtual ~PCG()
  {}

  // Member Functions

    //- Solve the matrix with this solver
    virtual solverPerformance solve
    (
      scalarField& psi,
      const scalarField& source,
      const direction cmpt=0
    ) const;

};

}  // namespace mousse
#endif
