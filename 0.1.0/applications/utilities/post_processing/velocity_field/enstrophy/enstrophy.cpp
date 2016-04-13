// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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
  if (Uheader.headerOk())
  {
    Info << "    Reading U" << endl;
    volVectorField U{Uheader, mesh};
    Info << "    Calculating enstrophy" << endl;
    volScalarField enstrophy
    {
      {
        "enstrophy",
        runTime.timeName(),
        mesh
      },
      0.5*magSqr(fvc::curl(U))
    };
    Info << "enstrophy(U) max/min : " << max(enstrophy).value() << " "
      << min(enstrophy).value() << endl;
    if (writeResults)
    {
      enstrophy.write();
    }
  }
  else
  {
    Info << "    No U" << endl;
  }
  Info << "\nEnd\n" << endl;
}
