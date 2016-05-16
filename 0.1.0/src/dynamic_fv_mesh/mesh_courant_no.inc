// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

scalar meshCoNum = 0.0;
scalar meanMeshCoNum = 0.0;
if (mesh.nInternalFaces()) {
  scalar dt = runTime.deltaTValue();
  scalarField sumPhi{fvc::surfaceSum(mag(mesh.phi()))().internalField()};
  meshCoNum = 0.5*gMax(sumPhi/mesh.V().field())*dt;
  meanMeshCoNum = 0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*dt;
}
Info << "Mesh Courant Number mean: " << meanMeshCoNum << " max: " << meshCoNum
  << endl;

