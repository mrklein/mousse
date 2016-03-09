// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "wall_fv_patch.hpp"

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
    IOobject Uheader
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    // Check U exists
    if (Uheader.headerOk())
    {
      mesh.readUpdate();
      Info << "    Reading U" << endl;
      volVectorField U{Uheader, mesh};
      Info << "    Calculating wallGradU" << endl;
      volVectorField wallGradU
      {
        {
          "wallGradU",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        mesh,
        {"wallGradU", U.dimensions()/dimLength, vector::zero}
      };
      const fvPatchList& patches = mesh.boundary();
      FOR_ALL(wallGradU.boundaryField(), patchi)
      {
        const fvPatch& currPatch = patches[patchi];
        if (isA<wallFvPatch>(currPatch))
        {
          wallGradU.boundaryField()[patchi] = -U.boundaryField()[patchi].snGrad();
        }
      }
      wallGradU.write();
    }
    else
    {
      Info << "    No U" << endl;
    }
  }
  Info << "End" << endl;
  return 0;
}
