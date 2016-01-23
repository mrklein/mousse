// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

scalar CoNum = 0.0;
scalar meanCoNum = 0.0;
scalar waveCoNum = 0.0;
if (mesh.nInternalFaces())
{
  scalarField sumPhi
  (
    fvc::surfaceSum(mag(phi))().internalField()
   / h.internalField()
  );
  CoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();
  meanCoNum =
    0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();
  // Gravity wave Courant number
  waveCoNum = 0.25*gMax
  (
    fvc::surfaceSum
    (
      fvc::interpolate(sqrt(h))*mesh.magSf()
    )().internalField()/mesh.V().field()
  )*sqrt(magg).value()*runTime.deltaTValue();
}
Info<< "Courant number mean: " << meanCoNum
  << " max: " << CoNum << endl;
Info<< "Gravity wave Courant number max: " << waveCoNum
  << endl;
