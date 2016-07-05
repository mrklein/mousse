// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  scalar meanStCoNum = 0.0;
  if (mesh.nInternalFaces()) {
   scalarField sumPhi
   {
     fvc::surfaceSum(mag(phiSt))().internalField()/rho.internalField()
   };
   StCoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();
   meanStCoNum =
    0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();
  }
  Info << "St Courant Number mean: " << meanStCoNum
    << " max: " << StCoNum << endl;
}
