// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Application
//   mousse-vorticity
// Description
//   Calculates and writes the vorticity of velocity field U.
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
    Info << "    Calculating vorticity" << endl;
    volVectorField vorticity
    {
      {
        "vorticity",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
      },
      fvc::curl(U)
    };
    volScalarField magVorticity
    {
      {
        "magVorticity",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
      },
      mag(vorticity)
    };
    Info << "vorticity max/min : "
      << max(magVorticity).value() << " "
      << min(magVorticity).value() << endl;
    if (writeResults) {
      vorticity.write();
      magVorticity.write();
    }
  } else {
    Info << "    No U" << endl;
  }
  Info << "\nEnd\n" << endl;
}

