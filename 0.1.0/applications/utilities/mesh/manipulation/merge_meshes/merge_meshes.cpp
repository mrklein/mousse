// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "merge_poly_mesh.hpp"
using namespace mousse;
void getRootCase(fileName& casePath)
{
  casePath.clean();
  if (casePath.empty() || casePath == ".")
  {
    // handle degenerate form and '.'
    casePath = cwd();
  }
  else if (casePath[0] != '/' && casePath.name() == "..")
  {
    // avoid relative cases ending in '..' - makes for very ugly names
    casePath = cwd()/casePath;
    casePath.clean();
  }
}
int main(int argc, char *argv[])
{
  argList::addNote
  (
    "merge two meshes"
  );
  argList::noParallel();
  #include "add_overwrite_option.inc"
  argList::validArgs.append("masterCase");
  argList::addOption
  (
    "masterRegion",
    "name",
    "specify alternative mesh region for the master mesh"
  );
  argList::validArgs.append("addCase");
  argList::addOption
  (
    "addRegion",
    "name",
    "specify alternative mesh region for the additional mesh"
  );
  argList args(argc, argv);
  if (!args.check())
  {
    FatalError.exit();
  }
  const bool overwrite = args.optionFound("overwrite");
  fileName masterCase = args[1];
  word masterRegion = polyMesh::defaultRegion;
  args.optionReadIfPresent("masterRegion", masterRegion);
  fileName addCase = args[2];
  word addRegion = polyMesh::defaultRegion;
  args.optionReadIfPresent("addRegion", addRegion);
  getRootCase(masterCase);
  getRootCase(addCase);
  Info<< "Master:      " << masterCase << "  region " << masterRegion << nl
    << "mesh to add: " << addCase    << "  region " << addRegion << endl;
  #include "create_times.inc"
  Info<< "Reading master mesh for time = " << runTimeMaster.timeName() << nl;
  Info<< "Create mesh\n" << endl;
  mergePolyMesh masterMesh
  {
    // IOobject
    {
      masterRegion,
      runTimeMaster.timeName(),
      runTimeMaster
    }
  };
  const word oldInstance = masterMesh.pointsInstance();
  Info<< "Reading mesh to add for time = " << runTimeToAdd.timeName() << nl;
  Info<< "Create mesh\n" << endl;
  polyMesh meshToAdd
  {
    // IOobject
    {
      addRegion,
      runTimeToAdd.timeName(),
      runTimeToAdd
    }
  };
  if (!overwrite)
  {
    runTimeMaster++;
  }
  Info<< "Writing combined mesh to " << runTimeMaster.timeName() << endl;
  masterMesh.addMesh(meshToAdd);
  masterMesh.merge();
  if (overwrite)
  {
    masterMesh.setInstance(oldInstance);
  }
  masterMesh.write();
  Info<< "\nEnd\n" << endl;
  return 0;
}
