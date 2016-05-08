// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc.hpp"
#include "time_selector.hpp"


namespace mousse {

void tryCalc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
  FatalIOError.throwExceptions();
  try {
    calc(args, runTime, mesh);
  } catch(IOerror& err) {
    Warning << err << endl;
  }
}

}


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
  #include "set_root_case.inc"
  #include "create_time.inc"
  mousse::instantList timeDirs = mousse::timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    mousse::Info << "Time = " << runTime.timeName() << mousse::endl;
    mesh.readUpdate();
    mousse::tryCalc(args, runTime, mesh);
    mousse::Info << mousse::endl;
  }
  return 0;
}

