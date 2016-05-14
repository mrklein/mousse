// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc.hpp"
#include "fvc.hpp"


void mousse::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
  bool writeResults = !args.optionFound("noWrite");
  IOobject phiHeader
  {
    "phi",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  if (phiHeader.headerOk()) {
    volScalarField Co
    {
      {
        "Co",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
      },
      mesh,
      {"0", dimless, 0},
      zeroGradientFvPatchScalarField::typeName
    };
    Info << "    Reading phi" << endl;
    surfaceScalarField phi{phiHeader, mesh};
    if (phi.dimensions() == dimensionSet{1, 0, -1, 0, 0}) {
      Info << "    Calculating compressible Co" << endl;
      Info << "    Reading rho" << endl;
      volScalarField rho
      {
        {
          "rho",
          runTime.timeName(),
          mesh,
          IOobject::MUST_READ
        },
        mesh
      };
      Co.dimensionedInternalField() =
        (0.5*runTime.deltaT())
        *fvc::surfaceSum(mag(phi))().dimensionedInternalField()
        /(rho*mesh.V());
      Co.correctBoundaryConditions();
    } else if (phi.dimensions() == dimensionSet{0, 3, -1, 0, 0}) {
      Info << "    Calculating incompressible Co" << endl;
      Co.dimensionedInternalField() =
        (0.5*runTime.deltaT())
        *fvc::surfaceSum(mag(phi))().dimensionedInternalField()/mesh.V();
      Co.correctBoundaryConditions();
    } else {
      FATAL_ERROR_IN(args.executable())
        << "Incorrect dimensions of phi: " << phi.dimensions()
        << abort(FatalError);
    }
    Info << "Co max : " << max(Co).value() << endl;
    if (writeResults) {
      Co.write();
    }
  } else {
    Info << "    No phi" << endl;
  }
  Info << "\nEnd\n" << endl;
}

