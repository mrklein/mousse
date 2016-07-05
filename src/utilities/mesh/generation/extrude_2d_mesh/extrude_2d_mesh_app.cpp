// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "extrude_2d_mesh.hpp"
#include "extrude_model.hpp"
#include "poly_topo_change.hpp"
#include "meshed_surface.hpp"
#include "edge_collapser.hpp"
#include "add_patch_cell_layer.hpp"
#include "patch_to_poly_2d_mesh.hpp"
#include "global_index.hpp"


using namespace mousse;


enum ExtrudeMode
{
  POLYMESH2D,
  MESHEDSURFACE
};


namespace mousse {

template<>
  const char* NamedEnum<ExtrudeMode, 2>::names[] =
  {
    "polyMesh2D",
    "MeshedSurface"
  };

}


static const NamedEnum<ExtrudeMode, 2> ExtrudeModeNames;
int main(int argc, char *argv[])
{
  argList::validArgs.append("surfaceFormat");
  #include "add_overwrite_option.inc"
  #include "set_root_case.inc"
  Info << "Create time\n" << endl;
  Time runTimeExtruded
  {
    Time::controlDictName,
    args.rootPath(),
    args.caseName()
  };
  runTimeExtruded.functionObjects().off();
  const ExtrudeMode surfaceFormat = ExtrudeModeNames[args[1]];
  const bool overwrite = args.optionFound("overwrite");
  Info << "Extruding from " << ExtrudeModeNames[surfaceFormat]
    << " at time " << runTimeExtruded.timeName() << endl;
  IOdictionary extrude2DMeshDict
  {
    IOobject
    {
      "extrude2DMeshDict",
      runTimeExtruded.system(),
      runTimeExtruded,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  // Point generator
  autoPtr<extrudeModel> model{extrudeModel::New(extrude2DMeshDict)};
  autoPtr<MeshedSurface<face>> fMesh;
  autoPtr<polyMesh> mesh;
  autoPtr<polyTopoChange> meshMod;
  labelListList extrudeEdgePatches;
  if (surfaceFormat == MESHEDSURFACE) {
    fMesh.set(new MeshedSurface<face>{"MeshedSurface.obj"});
    EdgeMap<label> edgeRegionMap;
    wordList patchNames{1, "default"};
    labelList patchSizes{1, fMesh().nEdges() - fMesh().nInternalEdges()};
    const edgeList& edges = fMesh().edges();
    FOR_ALL(edges, edgeI) {
      if (!fMesh().isInternalEdge(edgeI)) {
        edgeRegionMap.insert(edges[edgeI], 0);
      }
    }
    patchToPoly2DMesh poly2DMesh
    {
      fMesh(),
      patchNames,
      patchSizes,
      edgeRegionMap
    };
    poly2DMesh.createMesh();
    mesh.set
    (
      new polyMesh
      {
        {
          polyMesh::defaultRegion,
          runTimeExtruded.constant(),
          runTimeExtruded,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        xferMove(poly2DMesh.points()),
        xferMove(poly2DMesh.faces()),
        xferMove(poly2DMesh.owner()),
        xferMove(poly2DMesh.neighbour())
      }
    );
    Info << "Constructing patches." << endl;
    List<polyPatch*> patches{poly2DMesh.patchNames().size()};
    FOR_ALL(patches, patchI) {
      patches[patchI] =
        new polyPatch
        {
          poly2DMesh.patchNames()[patchI],
          poly2DMesh.patchSizes()[patchI],
          poly2DMesh.patchStarts()[patchI],
          patchI,
          mesh().boundaryMesh(),
          polyPatch::typeName
        };
    }
    mesh().addPatches(patches);
  } else if (surfaceFormat == POLYMESH2D) {
    mesh.set
    (
      new polyMesh
      {
        // IOobject
        {
          polyMesh::defaultRegion,
          runTimeExtruded.timeName(),
          runTimeExtruded,
          IOobject::MUST_READ
        }
      }
    );
  }
  // Engine to extrude mesh
  extrude2DMesh extruder{mesh(), extrude2DMeshDict, model()};
  extruder.addFrontBackPatches();
  meshMod.set(new polyTopoChange{mesh().boundaryMesh().size()});
  extruder.setRefinement(meshMod());
  // Create a mesh from topo changes.
  autoPtr<mapPolyMesh> morphMap = meshMod().changeMesh(mesh(), false);
  mesh().updateMesh(morphMap);

  {
    edgeCollapser collapser{mesh()};
    const edgeList& edges = mesh().edges();
    const pointField& points = mesh().points();
    const boundBox& bb = mesh().bounds();
    const scalar mergeDim = 1e-4*bb.minDim();
    PackedBoolList collapseEdge{mesh().nEdges()};
    Map<point> collapsePointToLocation{mesh().nPoints()};
    FOR_ALL(edges, edgeI) {
      const edge& e = edges[edgeI];
      scalar d = e.mag(points);
      if (d < mergeDim) {
        Info << "Merging edge " << e << " since length " << d
          << " << " << mergeDim << nl;
        collapseEdge[edgeI] = true;
        collapsePointToLocation.set(e[1], points[e[0]]);
      }
    }
    List<pointEdgeCollapse> allPointInfo;
    const globalIndex globalPoints{mesh().nPoints()};
    labelList pointPriority{mesh().nPoints(), 0};
    collapser.consistentCollapse
      (
        globalPoints,
        pointPriority,
        collapsePointToLocation,
        collapseEdge,
        allPointInfo
      );
    polyTopoChange meshModCollapse{mesh()};
    collapser.setRefinement(allPointInfo, meshModCollapse);
    // Create a mesh from topo changes.
    autoPtr<mapPolyMesh> morphMap = meshModCollapse.changeMesh(mesh(), false);
    mesh().updateMesh(morphMap);
  }
  if (!overwrite) {
    runTimeExtruded++;
  } else {
    mesh().setInstance("constant");
  }
  // Take over refinement levels and write to new time directory.
  Info << "\nWriting extruded mesh to time = " << runTimeExtruded.timeName()
    << nl << endl;
  mesh().write();
  Info << "End\n" << endl;
  return 0;
}

