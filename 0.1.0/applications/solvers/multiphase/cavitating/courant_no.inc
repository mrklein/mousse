// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

scalar CoNum = 0.0;
scalar meanCoNum = 0.0;
scalar acousticCoNum = 0.0;
if (mesh.nInternalFaces())
{
  scalarField sumPhi
  {
    fvc::surfaceSum(mag(phi))().internalField()
  };
  CoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();
  meanCoNum =
    0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();
  acousticCoNum = 0.5*gMax
  (
    fvc::surfaceSum
    (
      fvc::interpolate(scalar(1)/sqrt(psi))*mesh.magSf()
    )().internalField()/mesh.V().field()
  )*runTime.deltaTValue();
}
Info << "phi Courant Number mean: " << meanCoNum
  << " max: " << CoNum
  << " acoustic max: " << acousticCoNum
  << endl;
