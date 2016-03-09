// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Application
//   mousse-uprime
// Description
//   Calculates and writes the scalar field of uprime (sqrt(2/3 k)).
//   The -noWrite option just outputs the max/min values without writing
//   the field.

#include "calc.hpp"
#include "fvc.hpp"

void mousse::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
  bool writeResults = !args.optionFound("noWrite");
  IOobject kheader
  {
    "k",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  if (kheader.headerOk())
  {
    Info << "    Reading k" << endl;
    volScalarField k{kheader, mesh};
    Info << "    Calculating uprime" << endl;
    volScalarField uprime
    {
      {
        "uprime",
        runTime.timeName(),
        mesh
      },
      sqrt((2.0/3.0)*k)
    };
    Info << "uprime max/min : "
      << max(uprime).value() << " "
      << min(uprime).value() << endl;
    if (writeResults)
    {
      uprime.write();
    }
  }
  else
  {
    Info << "    No k" << endl;
  }
  Info << "\nEnd\n" << endl;
}
