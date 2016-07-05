// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "compressible_courant_no.hpp"
#include "fvc.hpp"


mousse::scalar mousse::compressibleCourantNo
(
  const fvMesh& mesh,
  const Time& runTime,
  const volScalarField& rho,
  const surfaceScalarField& phi
)
{
  scalarField sumPhi
  {
    fvc::surfaceSum(mag(phi))().internalField()/rho.internalField()
  };
  scalar CoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();
  scalar meanCoNum =
    0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();
  Info << "Region: " << mesh.name() << " Courant Number mean: " << meanCoNum
    << " max: " << CoNum << endl;
  return CoNum;
}
