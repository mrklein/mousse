// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Application
//   mousse-q
// Description
//   Calculates and writes the second invariant of the velocity gradient tensor.
//     Q = 0.5*(sqr(tr(gradU)) - tr(((gradU)&(gradU))))      [1/s^2]
//   The -noWrite option just outputs the max/min values without writing
//   the field.

#include "calc.hpp"
#include "fvc.hpp"


void mousse::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
  bool writeResults = !args.optionFound("noWrite");
  IOobject Uheader
  {
    "U",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  if (Uheader.headerOk()) {
    Info << "    Reading U" << endl;
    volVectorField U{Uheader, mesh};
    volTensorField gradU{fvc::grad(U)};
    volScalarField Q
    {
      {
        "Q",
        runTime.timeName(),
        mesh
      },
      0.5*(sqr(tr(gradU)) - tr(((gradU)&(gradU))))
    };
    Info << "mag(Q) max/min : "
      << max(Q).value() << " "
      << min(Q).value() << endl;
    if (writeResults) {
      Q.write();
    }
  } else {
    Info << "    No U" << endl;
  }
  Info << "\nEnd\n" << endl;
}

