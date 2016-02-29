// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

scalar CoNum = 0.0;
scalar meanCoNum = 0.0;
if (mesh.nInternalFaces())
{
  scalarField sumPhi
  {
    fvc::surfaceSum(mag(phi))().internalField()
  };
  FOR_ALL(phases, phasei)
  {
    sumPhi = max
    (
      sumPhi,
      fvc::surfaceSum(mag(phases[phasei].phi()))().internalField()
    );
  }
  CoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();
  meanCoNum = 0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();
}
Info << "Courant Number mean: " << meanCoNum
  << " max: " << CoNum << endl;
