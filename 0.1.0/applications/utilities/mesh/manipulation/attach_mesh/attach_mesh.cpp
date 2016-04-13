// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "attach_poly_topo_changer.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  argList::noParallel();
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const bool overwrite = args.optionFound("overwrite");
  if (!overwrite)
  {
    runTime++;
  }
  Info << "Time = " << runTime.timeName() << nl
    << "Attaching sliding interface" << endl;
  attachPolyTopoChanger(mesh).attach();
  if (overwrite)
  {
    mesh.setInstance(oldInstance);
  }
  mesh.write();
  Info<< "End\n" << endl;
  return 0;
}
