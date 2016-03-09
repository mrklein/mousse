// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Application
//   mousse-flow-type
// Description
//   Calculates and writes the flowType of velocity field U.
//   The -noWrite option has no meaning.
//   The flow type parameter is obtained according to the following equation:
//   \verbatim
//         |D| - |Omega|
//     lambda = -------------
//         |D| + |Omega|
//     -1 = rotational flow
//     0 = simple shear flow
//     1 = planar extensional flow
//   \endverbatim

#include "calc.hpp"
#include "fvc.hpp"

void mousse::calc(const argList& /*args*/, const Time& runTime, const fvMesh& mesh)
{
  IOobject Uheader
  {
    "U",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  if (Uheader.headerOk())
  {
    Info << "    Reading U" << endl;
    volVectorField U{Uheader, mesh};
    volTensorField gradU{fvc::grad(U)};
    volScalarField magD{mag(symm(gradU))};
    volScalarField magOmega{mag(skew(gradU))};
    dimensionedScalar smallMagD{"smallMagD", magD.dimensions(), SMALL};
    Info << "    Calculating flowType" << endl;
    volScalarField flowType
    {
      {
        "flowType",
        runTime.timeName(),
        mesh
      },
      (magD - magOmega)/(magD + magOmega + smallMagD)
    };
    flowType.write();
  }
  else
  {
    Info << "    No U" << endl;
  }
  Info << "\nEnd\n" << endl;
}
