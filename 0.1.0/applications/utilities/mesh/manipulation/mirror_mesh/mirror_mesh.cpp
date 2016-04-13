// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "mirror_fv_mesh.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  const bool overwrite = args.optionFound("overwrite");
  if (!overwrite)
  {
    runTime++;
  }
  mirrorFvMesh mesh
  {
    {
      mirrorFvMesh::defaultRegion,
      runTime.constant(),
      runTime
    }
  };
  // Set the precision of the points data to 10
  IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));
  Info << "Writing mirrored mesh" << endl;
  mesh.mirrorMesh().write();
  Info << "End" << endl;
  return 0;
}
