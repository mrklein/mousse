// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "fluent_fv_mesh.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  argList::noParallel();
  #include "set_root_case.inc"
  #include "create_time.inc"
  Info << "Create mesh for time = " << runTime.timeName() << nl << endl;
  fluentFvMesh mesh
  {
    {
      fluentFvMesh::defaultRegion,
      runTime.constant(),
      runTime
    }
  };
  mesh.writeFluentMesh();
  Info << "End\n" << endl;
  return 0;
}

