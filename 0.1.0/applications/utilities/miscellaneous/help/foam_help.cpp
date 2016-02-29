// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "help_type.hpp"
using namespace mousse;
int main(int argc, char *argv[])
{
  #include "add_region_option.hpp"
  if (argc < 2)
  {
    FatalError
      << "No help utility has been supplied" << nl
      << exit(FatalError);
  }
  const word utilityName = argv[1];
  mousse::autoPtr<mousse::helpType> utility
  (
    helpType::New(utilityName)
  );
  utility().init();
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  #include "create_named_mesh.hpp"
  utility().execute(args, mesh);
  Info<< "End\n" << endl;
}
