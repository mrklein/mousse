// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc.hpp"
#include "fluid_thermo.hpp"


inline bool hasThermo(const Time& time, const fvMesh& mesh)
{
  IOobject thermo
  {
    basicThermo::dictName,
    time.constant(),
    mesh
  };
  return thermo.headerOk();
}


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
  IOobject Theader
  {
    "T",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  // Check U and T exists
  if (Uheader.headerOk() && Theader.headerOk()) {
    autoPtr<volScalarField> MachPtr;
    volVectorField U{Uheader, mesh};
    if (hasThermo(runTime, mesh)) {
      // thermophysical Mach
      autoPtr<fluidThermo> thermo{fluidThermo::New(mesh)};
      volScalarField Cp{thermo->Cp()};
      volScalarField Cv{thermo->Cv()};
      MachPtr.set
      (
        new volScalarField
        {
          {
            "Ma",
            runTime.timeName(),
            mesh
          },
          mag(U)/(sqrt((Cp/Cv)*(Cp - Cv)*thermo->T()))
        }
      );
    } else {
      // thermodynamic Mach
      IOdictionary thermoProps
      {
        {
          "thermodynamicProperties",
          runTime.constant(),
          mesh,
          IOobject::MUST_READ_IF_MODIFIED
        }
      };
      dimensionedScalar R{thermoProps.lookup("R")};
      dimensionedScalar Cv{thermoProps.lookup("Cv")};
      volScalarField T{Theader, mesh};
      MachPtr.set
      (
        new volScalarField
        (
          {
            "Ma",
            runTime.timeName(),
            mesh
          },
          mag(U)/(sqrt(((Cv + R)/Cv)*R*T))
        )
      );
    }
    Info << "Mach max : " << max(MachPtr()).value() << endl;
    if (writeResults) {
      MachPtr().write();
    }
  } else {
    Info << "    Missing U or T" << endl;
  }
  Info << "\nEnd\n" << endl;
}

