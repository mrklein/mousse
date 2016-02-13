// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "time_selector.hpp"
#include "poly_topo_change.hpp"
#include "fv_mesh.hpp"
#include "poly_mesh_filter.hpp"
#include "face_set.hpp"
using namespace mousse;
int main(int argc, char *argv[])
{
  timeSelector::addOptions(true, false);
  argList::addNote
  (
    "Collapses small edges to a point.\n"
    "Optionally collapse small faces to a point and thin faces to an edge."
  );
  argList::addBoolOption
  (
    "collapseFaces",
    "Collapse small and sliver faces as well as small edges"
  );
  argList::addOption
  (
    "collapseFaceSet",
    "faceSet",
    "Collapse faces that are in the supplied face set"
  );
  #include "add_overwrite_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  instantList timeDirs = timeSelector::selectIfPresent(runTime, args);
  #include "create_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const bool overwrite = args.optionFound("overwrite");
  const bool collapseFaces = args.optionFound("collapseFaces");
  const bool collapseFaceSet = args.optionFound("collapseFaceSet");
  if (collapseFaces && collapseFaceSet)
  {
    FATAL_ERROR_IN("main(int, char*[])")
      << "Both face zone collapsing and face collapsing have been"
      << "selected. Choose only one of:" << nl
      << "    -collapseFaces" << nl
      << "    -collapseFaceSet <faceSet>"
      << abort(FatalError);
  }
  // maintain indirectPatchFaces if it is there (default) or force
  // (if collapseFaceSet option provided)
  word faceSetName{"indirectPatchFaces"};
  IOobject::readOption readFlag = IOobject::READ_IF_PRESENT;
  if (args.optionReadIfPresent("collapseFaceSet", faceSetName))
  {
    readFlag = IOobject::MUST_READ;
  }
  labelIOList pointPriority
  {
    {
      "pointPriority",
      runTime.timeName(),
      runTime,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    labelList{mesh.nPoints(), labelMin}
  };
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< "Time = " << runTime.timeName() << endl;
    autoPtr<polyMeshFilter> meshFilterPtr;
    label nBadFaces = 0;
    faceSet indirectPatchFaces
    {
      mesh,
      faceSetName,
      readFlag,
      IOobject::AUTO_WRITE
    };
    Info<< "Read faceSet " << indirectPatchFaces.name() << " with "
      << returnReduce(indirectPatchFaces.size(), sumOp<label>())
      << " faces" << endl;

    {
      meshFilterPtr.set(new polyMeshFilter{mesh, pointPriority});
      polyMeshFilter& meshFilter = meshFilterPtr();
      // newMesh will be empty until it is filtered
      const autoPtr<fvMesh>& newMesh = meshFilter.filteredMesh();
      // Filter small edges only. This reduces the number of faces so that
      // the face filtering is sped up.
      nBadFaces = meshFilter.filterEdges(0);

      {
        polyTopoChange meshMod{newMesh()};
        meshMod.changeMesh(mesh, false);
        polyMeshFilter::copySets(newMesh(), mesh);
      }

      pointPriority = meshFilter.pointPriority();
    }

    if (collapseFaceSet)
    {
      meshFilterPtr.reset(new polyMeshFilter{mesh, pointPriority});
      polyMeshFilter& meshFilter = meshFilterPtr();
      const autoPtr<fvMesh>& newMesh = meshFilter.filteredMesh();
      // Filter faces. Pass in the number of bad faces that are present
      // from the previous edge filtering to use as a stopping criterion.
      meshFilter.filter(indirectPatchFaces);

      {
        polyTopoChange meshMod{newMesh};
        meshMod.changeMesh(mesh, false);
        polyMeshFilter::copySets(newMesh(), mesh);
      }

      pointPriority = meshFilter.pointPriority();
    }

    if (collapseFaces)
    {
      meshFilterPtr.reset(new polyMeshFilter(mesh, pointPriority));
      polyMeshFilter& meshFilter = meshFilterPtr();
      const autoPtr<fvMesh>& newMesh = meshFilter.filteredMesh();
      // Filter faces. Pass in the number of bad faces that are present
      // from the previous edge filtering to use as a stopping criterion.
      meshFilter.filter(nBadFaces);

      {
        polyTopoChange meshMod{newMesh};
        meshMod.changeMesh(mesh, false);
        polyMeshFilter::copySets(newMesh(), mesh);
      }

      pointPriority = meshFilter.pointPriority();
    }

    // Write resulting mesh
    if (!overwrite)
    {
      runTime++;
    }
    else
    {
      mesh.setInstance(oldInstance);
    }
    Info<< nl << "Writing collapsed mesh to time " << runTime.timeName()
      << nl << endl;
    mesh.write();
    pointPriority.write();
  }
  Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
    << "  ClockTime = " << runTime.elapsedClockTime() << " s"
    << nl << endl;
  Info<< "End\n" << endl;
  return 0;
}
