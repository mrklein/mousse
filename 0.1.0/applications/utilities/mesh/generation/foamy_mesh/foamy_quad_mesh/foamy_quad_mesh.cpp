// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cv_2d.hpp"
#include "arg_list.hpp"
#include "meshed_surfaces.hpp"
#include "short_edge_filter_2d.hpp"
#include "extrude_2d_mesh.hpp"
#include "poly_mesh.hpp"
#include "patch_to_poly_2d_mesh.hpp"
#include "extrude_model.hpp"
#include "poly_topo_change.hpp"
#include "edge_collapser.hpp"
#include "global_index.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  argList::noParallel();
  argList::validArgs.clear();
  argList::validOptions.insert
  (
    "pointsFile",
    "filename"
  );
  #include "add_overwrite_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  // Read control dictionary
  IOdictionary controlDict
  {
    {
      "foamyQuadMeshDict",
      runTime.system(),
      runTime,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  const dictionary& shortEdgeFilterDict =
    controlDict.subDict("shortEdgeFilter");
  const dictionary& extrusionDict = controlDict.subDict("extrusion");
  Switch extrude{extrusionDict.lookup("extrude")};
  const bool overwrite = args.optionFound("overwrite");
  // Read and triangulation
  CV2D mesh{runTime, controlDict};
  if (args.options().found("pointsFile")) {
    fileName pointFileName{IStringStream{args.options()["pointsFile"]}()};
    mesh.insertPoints(pointFileName);
  } else {
    mesh.insertGrid();
  }
  mesh.insertSurfacePointPairs();
  mesh.boundaryConform();
  while (runTime.loop()) {
    Info << nl << "Time = " << runTime.timeName() << endl;
    mesh.newPoints();
  }
  mesh.write();
  Info << "Finished Delaunay in = " << runTime.cpuTimeIncrement() << " s."
    << endl;
  Info << "Begin filtering short edges:" << endl;
  shortEdgeFilter2D sef{mesh, shortEdgeFilterDict};
  sef.filter();
  Info << "Meshed surface after edge filtering :" << endl;
  sef.fMesh().writeStats(Info);
  if (mesh.meshControls().meshedSurfaceOutput()) {
    Info << "Write .obj file of the 2D mesh: MeshedSurface.obj" << endl;
    sef.fMesh().write("MeshedSurface.obj");
  }
  Info << "Finished filtering in = " << runTime.cpuTimeIncrement() << " s."
    << endl;
  Info << "Begin constructing a polyMesh:" << endl;
  patchToPoly2DMesh poly2DMesh
  {
    sef.fMesh(),
    sef.patchNames(),
    sef.patchSizes(),
    sef.mapEdgesRegion()
  };
  poly2DMesh.createMesh();
  polyMesh pMesh
  {
    {
      polyMesh::defaultRegion,
      runTime.constant(),
      runTime,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    xferMove(poly2DMesh.points()),
    xferMove(poly2DMesh.faces()),
    xferMove(poly2DMesh.owner()),
    xferMove(poly2DMesh.neighbour())
  };
  Info << "Constructing patches." << endl;
  List<polyPatch*> patches{poly2DMesh.patchNames().size()};
  label countPatches = 0;
  FOR_ALL(patches, patchI) {
    if (poly2DMesh.patchSizes()[patchI] == 0)
      continue;
    patches[countPatches] =
      new polyPatch
      {
        poly2DMesh.patchNames()[patchI],
        poly2DMesh.patchSizes()[patchI],
        poly2DMesh.patchStarts()[patchI],
        countPatches,
        pMesh.boundaryMesh(),
        word::null
      };
    countPatches++;
  }
  patches.setSize(countPatches);
  pMesh.addPatches(patches);
  if (extrude) {
    Info << "Begin extruding the polyMesh:" << endl;

    {
      // Point generator
      autoPtr<extrudeModel> model{extrudeModel::New(extrusionDict)};
      extrude2DMesh extruder{pMesh, extrusionDict, model()};
      extruder.addFrontBackPatches();
      polyTopoChange meshMod{pMesh.boundaryMesh().size()};
      extruder.setRefinement(meshMod);
      autoPtr<mapPolyMesh> morphMap = meshMod.changeMesh(pMesh, false);
      pMesh.updateMesh(morphMap);
    }
  }
  if (!overwrite) {
    runTime++;
  } else {
    pMesh.setInstance("constant");
  }
  pMesh.write();
  Info << "Finished extruding in = "
    << runTime.cpuTimeIncrement() << " s." << endl;
  Info << nl << "End\n" << endl;
  return 0;
}

