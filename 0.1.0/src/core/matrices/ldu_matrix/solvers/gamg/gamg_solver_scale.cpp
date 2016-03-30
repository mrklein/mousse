// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_solver.hpp"
#include "vector_2d.hpp"


// Private Member Functions 
void mousse::GAMGSolver::scale
(
  scalarField& field,
  scalarField& Acf,
  const lduMatrix& A,
  const FieldField<Field, scalar>& interfaceLevelBouCoeffs,
  const lduInterfaceFieldPtrsList& interfaceLevel,
  const scalarField& source,
  const direction cmpt
) const
{
  A.Amul
  (
    Acf,
    field,
    interfaceLevelBouCoeffs,
    interfaceLevel,
    cmpt
  );
  scalar scalingFactorNum = 0.0;
  scalar scalingFactorDenom = 0.0;
  FOR_ALL(field, i) {
    scalingFactorNum += source[i]*field[i];
    scalingFactorDenom += Acf[i]*field[i];
  }
  vector2D scalingVector(scalingFactorNum, scalingFactorDenom);
  A.mesh().reduce(scalingVector, sumOp<vector2D>());
  scalar sf = scalingVector.x()/stabilise(scalingVector.y(), VSMALL);
  if (debug >= 2) {
    Pout << sf << " ";
  }
  const scalarField& D = A.diag();
  FOR_ALL(field, i) {
    field[i] = sf*field[i] + (source[i] - sf*Acf[i])/D[i];
  }
}
