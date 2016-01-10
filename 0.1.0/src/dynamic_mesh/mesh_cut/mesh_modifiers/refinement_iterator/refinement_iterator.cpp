// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "refinement_iterator.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "refine_cell.hpp"
#include "undoable_mesh_cutter.hpp"
#include "poly_topo_change.hpp"
#include "map_poly_mesh.hpp"
#include "cell_cuts.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(refinementIterator, 0);
}
// Constructors 
// Construct from components
mousse::refinementIterator::refinementIterator
(
  polyMesh& mesh,
  undoableMeshCutter& meshRefiner,
  const cellLooper& cellWalker,
  const bool writeMesh
)
:
  edgeVertex(mesh),
  mesh_(mesh),
  meshRefiner_(meshRefiner),
  cellWalker_(cellWalker),
  writeMesh_(writeMesh)
{}
// Destructor 
mousse::refinementIterator::~refinementIterator()
{}
// Member Functions 
mousse::Map<mousse::label> mousse::refinementIterator::setRefinement
(
  const List<refineCell>& refCells
)
{
  Map<label> addedCells(2*refCells.size());
  Time& runTime = const_cast<Time&>(mesh_.time());
  label nRefCells = refCells.size();
  label oldRefCells = -1;
  // Operate on copy.
  List<refineCell> currentRefCells(refCells);
  bool stop = false;
  do
  {
    if (writeMesh_)
    {
      // Need different times to write meshes.
      runTime++;
    }
    polyTopoChange meshMod(mesh_);
    if (debug)
    {
      Pout<< "refinementIterator : refining "
        << currentRefCells.size() << " cells." << endl;
    }
    // Determine cut pattern.
    cellCuts cuts(mesh_, cellWalker_, currentRefCells);
    label nCuts = cuts.nLoops();
    reduce(nCuts, sumOp<label>());
    if (nCuts == 0)
    {
      if (debug)
      {
        Pout<< "refinementIterator : exiting iteration since no valid"
          << " loops found for " << currentRefCells.size()
          << " cells" << endl;
        fileName cutsFile("failedCuts_" + runTime.timeName() + ".obj");
        Pout<< "Writing cuts for time " <<  runTime.timeName()
          << " to " << cutsFile << endl;
        OFstream cutsStream(cutsFile);
        labelList refCells(currentRefCells.size());
        FOR_ALL(currentRefCells, i)
        {
          refCells[i] = currentRefCells[i].cellNo();
        }
        meshTools::writeOBJ
        (
          cutsStream,
          mesh().cells(),
          mesh().faces(),
          mesh().points(),
          refCells
        );
      }
      break;
    }
    if (debug)
    {
      fileName cutsFile("cuts_" + runTime.timeName() + ".obj");
      Pout<< "Writing cuts for time " <<  runTime.timeName()
        << " to " << cutsFile << endl;
      OFstream cutsStream(cutsFile);
      cuts.writeOBJ(cutsStream);
    }
    // Insert mesh refinement into polyTopoChange.
    meshRefiner_.setRefinement(cuts, meshMod);
    //
    // Do all changes
    //
    autoPtr<mapPolyMesh> morphMap = meshMod.changeMesh
    (
      mesh_,
      false
    );
    // Move mesh (since morphing does not do this)
    if (morphMap().hasMotionPoints())
    {
      mesh_.movePoints(morphMap().preMotionPoints());
    }
    // Update stored refinement pattern
    meshRefiner_.updateMesh(morphMap());
    // Write resulting mesh
    if (writeMesh_)
    {
      if (debug)
      {
        Pout<< "Writing refined polyMesh to time "
          << runTime.timeName() << endl;
      }
      mesh_.write();
    }
    // Update currentRefCells for new cell numbers. Use helper function
    // in meshCutter class.
    updateLabels
    (
      morphMap->reverseCellMap(),
      currentRefCells
    );
    // Update addedCells for new cell numbers
    updateLabels
    (
      morphMap->reverseCellMap(),
      addedCells
    );
    // Get all added cells from cellCutter (already in new numbering
    // from meshRefiner.updateMesh call) and add to global list of added
    const Map<label>& addedNow = meshRefiner_.addedCells();
    FOR_ALL_CONST_ITER(Map<label>, addedNow, iter)
    {
      if (!addedCells.insert(iter.key(), iter()))
      {
        FATAL_ERROR_IN("refinementIterator")
          << "Master cell " << iter.key()
          << " already has been refined" << endl
          << "Added cell:" << iter() << abort(FatalError);
      }
    }
    // Get failed refinement in new cell numbering and reconstruct input
    // to the meshRefiner. Is done by removing all refined cells from
    // current list of cells to refine.
    // Update refCells for new cell numbers.
    updateLabels
    (
      morphMap->reverseCellMap(),
      currentRefCells
    );
    // Pack refCells acc. to refined status
    nRefCells = 0;
    FOR_ALL(currentRefCells, refI)
    {
      const refineCell& refCell = currentRefCells[refI];
      if (!addedNow.found(refCell.cellNo()))
      {
        if (nRefCells != refI)
        {
          currentRefCells[nRefCells++] =
            refineCell
            (
              refCell.cellNo(),
              refCell.direction()
            );
        }
      }
    }
    oldRefCells = currentRefCells.size();
    currentRefCells.setSize(nRefCells);
    if (debug)
    {
      Pout<< endl;
    }
    // Stop only if all finished or all can't refine any further.
    stop = (nRefCells == 0) || (nRefCells == oldRefCells);
    reduce(stop, andOp<bool>());
  }
  while (!stop);
  if (nRefCells == oldRefCells)
  {
    WARNING_IN("refinementIterator")
      << "stopped refining."
      << "Did not manage to refine a single cell" << endl
      << "Wanted :" << oldRefCells << endl;
  }
  return addedCells;
}
