// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "poly_mesh_zip_up_cells.hpp"
#include "bool_list.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_named_poly_mesh.inc"
  if (polyMeshZipUpCells(mesh))
  {
    Info << "Writing zipped-up polyMesh to " << mesh.facesInstance() << endl;
    mesh.write();
  }
  Info << "End\n" << endl;
  return 0;
}
