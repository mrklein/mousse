// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (mesh.nInternalFaces())
{
  scalarField sumAmaxSf{fvc::surfaceSum(amaxSf)().internalField()};
  CoNum = 0.5*gMax(sumAmaxSf/mesh.V().field())*runTime.deltaTValue();
  meanCoNum =
    0.5*(gSum(sumAmaxSf)/gSum(mesh.V().field()))*runTime.deltaTValue();
}
Info << "Mean and max Courant Numbers = " << meanCoNum << " " << CoNum << endl;
