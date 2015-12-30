// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "fv_io_option_list.hpp"
#include "simple_control.hpp"
int main(int argc, char *argv[])
{
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  #include "create_mesh.hpp"
  simpleControl simple(mesh);
  #include "create_fields.inc"
  #include "create_fv_options.hpp"
    Info<< "\nCalculating scalar transport\n" << endl;
  #include "courant_no.hpp"
  while (simple.loop())
  {
    Info<< "Time = " << runTime.timeName() << nl << endl;
    while (simple.correctNonOrthogonal())
    {
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
  Info<< "End\n" << endl;
  return 0;
}
