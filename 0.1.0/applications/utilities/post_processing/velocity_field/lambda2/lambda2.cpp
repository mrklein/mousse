// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Application
//   mousse-lambda2
// Description
//   Calculates and writes the second largest eigenvalue of the sum of the
//   square of the symmetrical and anti-symmetrical parts of the velocity
//   gradient tensor.
//   The -noWrite option has no meaning.

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
    const volTensorField gradU{fvc::grad(U)};
    volTensorField SSplusWW
    {
      (symm(gradU) & symm(gradU)) + (skew(gradU) & skew(gradU))
    };
    volScalarField Lambda2
    {
      {
        "Lambda2",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      -eigenValues(SSplusWW)().component(vector::Y)
    };
    Info << "    Writing -Lambda2" << endl;
    Lambda2.write();
  }
  else
  {
    Info << "    No U" << endl;
  }
  Info << "\nEnd\n" << endl;
}
