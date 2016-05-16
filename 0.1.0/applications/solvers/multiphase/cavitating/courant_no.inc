// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

scalar CoNum = 0.0;
scalar meanCoNum = 0.0;
scalar acousticCoNum = 0.0;

if (mesh.nInternalFaces()) {
  const auto& deltaT = runTime.deltaTValue();
  const auto& V = mesh.V().field();
  scalarField sumPhi{fvc::surfaceSum(mag(phi))().internalField()};
  CoNum = 0.5*gMax(sumPhi/V)*deltaT;
  meanCoNum = 0.5*(gSum(sumPhi)/gSum(V))*deltaT;
  acousticCoNum =
    0.5*gMax(
      fvc::surfaceSum
      (
        fvc::interpolate(scalar(1)/sqrt(psi))*mesh.magSf()
      )().internalField()/V
    )*deltaT;
}
Info << "phi Courant Number mean: " << meanCoNum
  << " max: " << CoNum
  << " acoustic max: " << acousticCoNum
  << endl;
