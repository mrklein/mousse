// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "iodictionary.hpp"
#include "searchable_surfaces.hpp"
#include "conformal_voronoi_mesh.hpp"
#include "vtk_set_writer.hpp"
using namespace mousse;
int main(int argc, char *argv[])
{
  mousse::argList::addBoolOption
  (
    "checkGeometry",
    "check all surface geometry for quality"
  );
  mousse::argList::addBoolOption
  (
    "conformationOnly",
    "conform to the initial points without any point motion"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  const bool checkGeometry = args.optionFound("checkGeometry");
  const bool conformationOnly = args.optionFound("conformationOnly");
  IOdictionary foamyHexMeshDict
  {
    {
      args.executable() + "Dict",
      runTime.system(),
      runTime,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  if (checkGeometry)
  {
    const searchableSurfaces allGeometry
    {
      {
        "cvSearchableSurfaces",
        runTime.constant(),
        "triSurface",
        runTime,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      },
      foamyHexMeshDict.subDict("geometry"),
      foamyHexMeshDict.lookupOrDefault("singleRegionName", true)
    };
    // Write some stats
    allGeometry.writeStats(List<wordList>(0), Info);
    // Check topology
    allGeometry.checkTopology(true);
    // Check geometry
    allGeometry.checkGeometry
    (
      100.0,      // max size ratio
      1e-9,       // intersection tolerance
      autoPtr<writer<scalar> >(new vtkSetWriter<scalar>()),
      0.01,       // min triangle quality
      true
    );
    return 0;
  }
  conformalVoronoiMesh::debug = true;
  Info<< "Create mesh for time = " << runTime.timeName() << nl << endl;
  conformalVoronoiMesh mesh{runTime, foamyHexMeshDict};
  if (conformationOnly)
  {
    mesh.initialiseForConformation();
    runTime++;
    mesh.writeMesh(runTime.timeName());
  }
  else
  {
    mesh.initialiseForMotion();
    while (runTime.run())
    {
      runTime++;
      Info<< nl << "Time = " << runTime.timeName() << endl;
      mesh.move();
      Info<< nl
        << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;
    }
  }
  Info<< nl << "End" << nl << endl;
  return 0;
}
