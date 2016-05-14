// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "graph.hpp"
#include "ofstream.hpp"
#include "kmesh.hpp"
#include "turb_gen.hpp"
#include "calc_ek.hpp"


int main(int argc, char *argv[])
{
  argList::noParallel();
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "create_fields.inc"
  #include "read_box_turb_dict.inc"

  Kmesh K{mesh};
  turbGen Ugen{K, Ea, k0};
  U.internalField() = Ugen.U();
  U.correctBoundaryConditions();
  Info << "k(" << runTime.timeName() << ") = "
    << 3.0/2.0*average(magSqr(U)).value() << endl;
  U.write();
  calcEk(U, K).write
  (
    runTime.path()/"graphs"/runTime.timeName(),
    "Ek",
    runTime.graphFormat()
  );
  Info << "end" << endl;
  return 0;
}

