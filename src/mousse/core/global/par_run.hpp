#ifndef CORE_GLOBAL_PAR_RUN_HPP_
#define CORE_GLOBAL_PAR_RUN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParRunControl
// Description
//   Helper class for initializing parallel jobs from the command arguments.

#include "pstream.hpp"
#include "iostreams.hpp"


namespace mousse {

class ParRunControl
{
  bool RunPar;
public:
  ParRunControl()
  :
    RunPar{false}
  {}
  ~ParRunControl()
  {
    if (RunPar) {
      Info << "Finalising parallel run" << endl;
      Pstream::exit(0);
    }
  }
  void runPar(int& argc, char**& argv)
  {
    RunPar = true;
    if (!Pstream::init(argc, argv)) {
      Info << "Failed to start parallel run" << endl;
      Pstream::exit(1);
    }
  }
  bool parRun() const
  {
    return RunPar;
  }
};

}  // namespace mousse

#endif
