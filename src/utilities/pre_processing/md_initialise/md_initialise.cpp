// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "md.hpp"
#include "fv_cfd.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  IOdictionary mdInitialiseDict
  {
    {
      "mdInitialiseDict",
      runTime.system(),
      runTime,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    }
  };
  IOdictionary idListDict
  {
    {
      "idList",
      mesh.time().constant(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    }
  };
  potential pot{mesh, mdInitialiseDict, idListDict};
  moleculeCloud molecules{mesh, pot, mdInitialiseDict};
  label totalMolecules = molecules.size();
  if (Pstream::parRun()) {
    reduce(totalMolecules, sumOp<label>());
  }
  Info << nl << "Total number of molecules added: " << totalMolecules
    << nl << endl;
  IOstream::defaultPrecision(15);
  if (!mesh.write()) {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing moleculeCloud."
      << nl << exit(FatalError);
  }
  Info << nl << "ClockTime = " << runTime.elapsedClockTime() << " s"
    << nl << endl;
  Info << "\nEnd\n" << endl;
  return 0;
}

