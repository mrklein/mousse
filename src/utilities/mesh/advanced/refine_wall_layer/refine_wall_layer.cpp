// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_topo_change.hpp"
#include "cell_cuts.hpp"
#include "cell_set.hpp"
#include "mesh_cutter.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  argList::noParallel();
  argList::validArgs.append("patches");
  argList::validArgs.append("edgeFraction");
  argList::addOption
  (
    "useSet",
    "name",
    "restrict cells to refine based on specified cellSet name"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  // Find set of patches from the list of regular expressions provided
  const wordReList patches{IStringStream{args[1]}()};
  const labelHashSet patchSet{mesh.boundaryMesh().patchSet(patches)};
  const scalar weight  = args.argRead<scalar>(2);
  const bool overwrite = args.optionFound("overwrite");
  if (!patchSet.size()) {
    FATAL_ERROR_IN(args.executable())
      << "Cannot find any patches in set " << patches << endl
      << "Valid patches are " << mesh.boundaryMesh().names()
      << exit(FatalError);
  }
  label nPatchFaces = 0;
  label nPatchEdges = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchSet, iter) {
    nPatchFaces += mesh.boundaryMesh()[iter.key()].size();
    nPatchEdges += mesh.boundaryMesh()[iter.key()].nEdges();
  }
  // Construct from estimate for the number of cells to refine
  labelHashSet cutCells{4*nPatchFaces};
  // Construct from total patch edges in selected patches
  DynamicList<label> allCutEdges{nPatchEdges};
  DynamicList<scalar> allCutEdgeWeights{nPatchEdges};
  // Find cells to refine
  FOR_ALL_CONST_ITER(labelHashSet, patchSet, iter) {
    const polyPatch& pp = mesh.boundaryMesh()[iter.key()];
    const labelList& meshPoints = pp.meshPoints();
    FOR_ALL(meshPoints, pointI) {
      label meshPointI = meshPoints[pointI];
      const labelList& pCells = mesh.pointCells()[meshPointI];
      FOR_ALL(pCells, pCellI) {
        cutCells.insert(pCells[pCellI]);
      }
    }
  }
  // Edit list of cells to refine according to specified set
  word setName;
  if (args.optionReadIfPresent("useSet", setName)) {
    Info << "Subsetting cells to cut based on cellSet"
      << setName << nl << endl;
    cellSet cells{mesh, setName};
    Info << "Read " << cells.size() << " cells from cellSet "
      << cells.instance()/cells.local()/cells.name()
      << nl << endl;
    FOR_ALL_CONST_ITER(cellSet, cells, iter) {
      cutCells.erase(iter.key());
    }
    Info << "Removed from cells to cut all the ones not in set "
      << setName << nl << endl;
  }
  // Mark all mesh points on patch
  boolList vertOnPatch{mesh.nPoints(), false};
  FOR_ALL_CONST_ITER(labelHashSet, patchSet, iter) {
    const polyPatch& pp = mesh.boundaryMesh()[iter.key()];
    const labelList& meshPoints = pp.meshPoints();
    FOR_ALL(meshPoints, pointI) {
      vertOnPatch[meshPoints[pointI]] = true;
    }
  }
  FOR_ALL_CONST_ITER(labelHashSet, patchSet, iter) {
    const polyPatch& pp = mesh.boundaryMesh()[iter.key()];
    const labelList& meshPoints = pp.meshPoints();
    FOR_ALL(meshPoints, pointI) {
      label meshPointI = meshPoints[pointI];
      const labelList& pEdges = mesh.pointEdges()[meshPointI];
      FOR_ALL(pEdges, pEdgeI) {
        const label edgeI = pEdges[pEdgeI];
        const edge& e = mesh.edges()[edgeI];
        label otherPointI = e.otherVertex(meshPointI);
        if (!vertOnPatch[otherPointI]) {
          allCutEdges.append(edgeI);
          if (e.start() == meshPointI) {
            allCutEdgeWeights.append(weight);
          } else {
            allCutEdgeWeights.append(1 - weight);
          }
        }
      }
    }
  }
  allCutEdges.shrink();
  allCutEdgeWeights.shrink();
  Info << "Refining:" << nl
    << "    cells:" << cutCells.size() << nl
    << "    edges:" << allCutEdges.size() << endl;
  // Transfer DynamicLists to straight ones.
  scalarField cutEdgeWeights;
  cutEdgeWeights.transfer(allCutEdgeWeights);
  allCutEdgeWeights.clear();
  // Gets cuts across cells from cuts through edges.
  cellCuts cuts
  {
    mesh,
    cutCells.toc(),     // cells candidate for cutting
    labelList{0},       // cut vertices
    allCutEdges,        // cut edges
    cutEdgeWeights      // weight on cut edges
  };
  polyTopoChange meshMod{mesh};
  // Cutting engine
  meshCutter cutter{mesh};
  // Insert mesh refinement into polyTopoChange.
  cutter.setRefinement(cuts, meshMod);
  if (!overwrite) {
    runTime++;
  }
  autoPtr<mapPolyMesh> morphMap = meshMod.changeMesh(mesh, false);
  if (morphMap().hasMotionPoints()) {
    mesh.movePoints(morphMap().preMotionPoints());
  }
  // Update stored labels on meshCutter.
  cutter.updateMesh(morphMap());
  Info << "Finished refining" << endl;
  if (overwrite) {
    mesh.setInstance(oldInstance);
  }
  // Write resulting mesh
  Info << "Writing refined mesh to time " << runTime.timeName() << endl;
  mesh.write();
  Info << "End\n" << endl;
  return 0;
}

