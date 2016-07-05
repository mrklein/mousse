#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_PBICG_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_PBICG_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PBiCG
// Description
//   Preconditioned bi-conjugate gradient solver for asymmetric lduMatrices
//   using a run-time selectable preconditiioner.

#include "ldu_matrix.hpp"


namespace mousse {

class PBiCG
:
  public lduMatrix::solver
{
public:

  //- Runtime type information
  TYPE_NAME("PBiCG");

  // Constructors

    //- Construct from matrix components and solver data stream
    PBiCG
    (
      const word& fieldName,
      const lduMatrix& matrix,
      const FieldField<Field, scalar>& interfaceBouCoeffs,
      const FieldField<Field, scalar>& interfaceIntCoeffs,
      const lduInterfaceFieldPtrsList& interfaces,
      const dictionary& solverControls
    );

    //- Disallow default bitwise copy construct
    PBiCG(const PBiCG&) = delete;

    //- Disallow default bitwise assignment
    PBiCG& operator=(const PBiCG&) = delete;

  //- Destructor
  virtual ~PBiCG()
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
