// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "tabulated_wall_function.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  IOdictionary dict
  {
    {
      "wallFunctionDict",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  autoPtr<tabulatedWallFunctions::tabulatedWallFunction>
    twf{tabulatedWallFunctions::tabulatedWallFunction::New(dict, mesh)};
  twf->write();
  Info << "End\n" << endl;
  return 0;
}
