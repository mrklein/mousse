// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "time.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  argList::addNote("List times using timeSelector");
  timeSelector::addOptions(true, true);
  argList::noBanner();
  argList::noParallel();
  argList::addBoolOption
  (
    "processor",
    "list times from processor0/ directory"
  );
  argList::addBoolOption
  (
    "rm",
    "remove selected time directories"
  );
  #include "set_root_case.inc"
  label nProcs = 0;
  // Create the processor databases
  PtrList<Time> databases{1};
  if (args.optionFound("processor")) {
    // Determine the processor count directly
    while (isDir(args.path()/(word("processor") + name(nProcs)))) {
      ++nProcs;
    }
    if (!nProcs) {
      FATAL_ERROR_IN(args.executable())
        << "No processor* directories found"
        << exit(FatalError);
    }
    // Create the processor databases
    databases.setSize(nProcs);
    FOR_ALL(databases, procI) {
      databases.set
      (
        procI,
        new Time
        {
          Time::controlDictName,
          args.rootPath(),
          args.caseName()/fileName(word("processor") + name(procI))
        }
      );
    }
  } else {
    databases.set
    (
      0,
      new Time
      {
        Time::controlDictName,
        args.rootPath(),
        args.caseName()
      }
    );
  }
  // Use the times list from the master processor
  // and select a subset based on the command-line options
  instantList timeDirs = timeSelector::select(databases[0].times(), args);
  if (args.optionFound("rm")) {
    if (args.optionFound("processor")) {
      for (label procI=0; procI<nProcs; procI++) {
        fileName procPath{args.path()/(word("processor") + name(procI))};
        FOR_ALL(timeDirs, timeI) {
          rmDir(procPath/timeDirs[timeI].name());
        }
      }
    } else {
      FOR_ALL(timeDirs, timeI) {
        rmDir(args.path()/timeDirs[timeI].name());
      }
    }
  } else {
    FOR_ALL(timeDirs, timeI) {
      Info << timeDirs[timeI].name() << endl;
    }
  }
  return 0;
}

