// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"

int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_mesh.inc"
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    mesh.readUpdate();
    volScalarField mgb
    {
      {
        "mgb",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      },
      mesh
    };
    volScalarField Su
    {
      {
        "Su",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      },
      mesh
    };
    volScalarField Xi
    {
      {
        "Xi",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      },
      mesh
    };
    volScalarField St
    {
      {
        "St",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
      },
      Xi*Su
    };
    St.write();
    volScalarField wdot
    {
      {
        "wdot",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ
      },
      St*mgb
    };
    wdot.write();
  }
  return 0;
}
