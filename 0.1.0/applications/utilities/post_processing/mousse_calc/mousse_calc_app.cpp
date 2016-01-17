// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_selector.hpp"
#include "calc_type.hpp"
int main(int argc, char *argv[])
{
  mousse::timeSelector::addOptions();
  #include "add_region_option.inc"
  mousse::argList::addBoolOption
  (
    "noWrite",
    "suppress writing results"
  );
  #include "add_dict_option.inc"
  if (argc < 2)
  {
    FatalError
      << "No utility has been supplied" << nl
      << exit(FatalError);
  }
  const word utilityName = argv[1];
  mousse::autoPtr<mousse::calcType> utility
  (
    calcType::New(utilityName)
  );
  utility().tryInit();
  #include "set_root_case.inc"
  #include "create_time.inc"
  mousse::instantList timeDirs = mousse::timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  utility().tryPreCalc(args, runTime, mesh);
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    mousse::Info<< "Time = " << runTime.timeName() << mousse::endl;
    mesh.readUpdate();
    utility().tryCalc(args, runTime, mesh);
    mousse::Info<< mousse::endl;
  }
  utility().tryPostCalc(args, runTime, mesh);
  Info<< "End\n" << endl;
  return 0;
}
