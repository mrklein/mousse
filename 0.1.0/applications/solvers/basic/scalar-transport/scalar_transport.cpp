// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "fv_io_option_list.hpp"
#include "simple_control.hpp"


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  simpleControl simple(mesh);
  #include "create_fields.inc"
  #include "create_fv_options.inc"
  Info << "\nCalculating scalar transport\n" << endl;
  #include "courant_no.inc"
  while (simple.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;
    while (simple.correctNonOrthogonal()) {
      solve
      (
        fvm::ddt(T)
      + fvm::div(phi, T)
      - fvm::laplacian(DT, T)
      ==
        fvOptions(T)
      );
    }
    runTime.write();
  }
  Info << "End\n" << endl;
  return 0;
}

