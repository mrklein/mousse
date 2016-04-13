// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"

int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    IOobject pheader
    {
      "p",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    IOobject Uheader
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    // Check p and U exist
    if (pheader.headerOk() && Uheader.headerOk())
    {
      mesh.readUpdate();
      Info << "    Reading p" << endl;
      volScalarField p(pheader, mesh);
      Info << "    Reading U" << endl;
      volVectorField U(Uheader, mesh);
      Info << "    Calculating ptot" << endl;
      if (p.dimensions() == dimensionSet(0, 2, -2, 0, 0))
      {
        volScalarField ptot
        {
          {
            "ptot",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ
          },
          p + 0.5*magSqr(U)
        };
        ptot.write();
      }
      else
      {
        IOobject rhoheader
        {
          "rho",
          runTime.timeName(),
          mesh,
          IOobject::MUST_READ
        };
        // Check rho exists
        if (rhoheader.headerOk())
        {
          Info << "    Reading rho" << endl;
          volScalarField rho{rhoheader, mesh};
          volScalarField ptot
          {
            {
              "ptot",
              runTime.timeName(),
              mesh,
              IOobject::NO_READ
            },
            p + 0.5*rho*magSqr(U)
          };
          ptot.write();
        }
        else
        {
          Info << "    No rho" << endl;
        }
      }
    }
    else
    {
      Info << "    No p or U" << endl;
    }
    Info << endl;
  }
  return 0;
}
