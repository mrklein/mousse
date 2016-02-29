#ifndef CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_GAMG_PRECONDITIONER_HPP_
#define CORE_MATRICES_LDU_MATRIX_PRECONDITIONERS_GAMG_PRECONDITIONER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GAMGPreconditioner
// Description
//   Geometric agglomerated algebraic multigrid preconditioner.
// See Also
//   GAMGSolver for more details.
// SourceFiles
//   gamg_preconditioner.cpp


#include "gamg_solver.hpp"

namespace mousse
{
class GAMGPreconditioner
:
  public GAMGSolver,
  public lduMatrix::preconditioner
{
protected:
  // Protected data
    //- Number of V-cycles to perform
    label nVcycles_;
    //- Read the control parameters from the controlDict_
    virtual void readControls();
public:
  //- Runtime type information
  TYPE_NAME("GAMG");
  // Constructors
    //- Construct from matrix components and preconditioner solver controls
    GAMGPreconditioner
    (
      const lduMatrix::solver&,
      const dictionary& solverControls
    );
  //- Destructor
  virtual ~GAMGPreconditioner();
  // Member Functions
    //- Return wA the preconditioned form of residual rA
    virtual void precondition
    (
      scalarField& wA,
      const scalarField& rA,
      const direction cmpt=0
    ) const;
};
}  // namespace mousse
#endif
