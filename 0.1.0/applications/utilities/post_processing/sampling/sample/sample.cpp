// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "io_sampled_sets.hpp"
#include "io_sampled_surfaces.hpp"
using namespace mousse;
int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.hpp"
  #include "add_dict_option.hpp"
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.hpp"
  const word dictName("sampleDict");
  autoPtr<IOsampledSets> sSetsPtr;
  autoPtr<IOsampledSurfaces> sSurfsPtr;
  if (args.optionFound("dict"))
  {
    // Construct from fileName
    fileName dictPath = args["dict"];
    if (isDir(dictPath))
    {
      dictPath = dictPath / dictName;
    }
    sSetsPtr.reset
    (
      new IOsampledSets
      (
        sampledSets::typeName,
        mesh,
        dictPath,
        IOobject::MUST_READ_IF_MODIFIED,
        true
      )
    );
    // Note: both IOsampledSets and IOsampledSurfaces read the
    //       same dictionary. Unregister one to make sure no duplicates
    //       trying to register
    sSetsPtr().checkOut();
    sSurfsPtr.reset
    (
      new IOsampledSurfaces
      (
        sampledSurfaces::typeName,
        mesh,
        dictPath,
        IOobject::MUST_READ_IF_MODIFIED,
        true
      )
    );
  }
  else
  {
    // Construct from name in system() directory
    sSetsPtr.reset
    (
      new IOsampledSets
      (
        sampledSets::typeName,
        mesh,
        dictName,
        IOobject::MUST_READ_IF_MODIFIED,
        true
      )
    );
    sSetsPtr().checkOut();
    sSurfsPtr.reset
    (
      new IOsampledSurfaces
      (
        sampledSurfaces::typeName,
        mesh,
        dictName,
        IOobject::MUST_READ_IF_MODIFIED,
        true
      )
    );
  }
  IOsampledSets& sSets = sSetsPtr();
  IOsampledSurfaces& sSurfs = sSurfsPtr();
  forAll(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< "Time = " << runTime.timeName() << endl;
    // Handle geometry/topology changes
    polyMesh::readUpdateState state = mesh.readUpdate();
    sSets.readUpdate(state);
    sSurfs.readUpdate(state);
    sSets.write();
    sSurfs.write();
    Info<< endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
