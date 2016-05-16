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
  runTime.setTime(timeDirs.last(), timeDirs.size()-1);
  volScalarField pMean
  {
    {
      "pMean",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    },
    mesh
  };
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    IOobject pheader
    {
      "p",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    // Check p exists
    if (pheader.headerOk()) {
      mesh.readUpdate();
      Info << "    Reading p" << endl;
      volScalarField p{pheader, mesh};
      Info << "    Calculating pPrime2" << endl;
      volScalarField pPrime2
      {
        {
          "pPrime2",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        sqr(p - pMean)
      };
      pPrime2.write();
    } else {
      Info << "    No p" << endl;
    }
    Info << endl;
  }
  return 0;
}

