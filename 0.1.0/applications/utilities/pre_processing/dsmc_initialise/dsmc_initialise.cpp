// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "dsmc_cloud.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  IOdictionary dsmcInitialiseDict
  {
    {
      "dsmcInitialiseDict",
      mesh.time().system(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  Info << "Initialising dsmc for Time = " << runTime.timeName() << nl << endl;
  dsmcCloud dsmc{"dsmc", mesh, dsmcInitialiseDict};
  label totalMolecules = dsmc.size();
  if (Pstream::parRun()) {
    reduce(totalMolecules, sumOp<label>());
  }
  Info << nl << "Total number of molecules added: " << totalMolecules
    << nl << endl;
  IOstream::defaultPrecision(15);
  if (!mesh.write()) {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing dsmcCloud."
      << nl << exit(FatalError);
  }
  Info << nl << "ClockTime = " << runTime.elapsedClockTime() << " s"
    << nl << endl;
  Info << "End\n" << endl;
  return 0;
}

