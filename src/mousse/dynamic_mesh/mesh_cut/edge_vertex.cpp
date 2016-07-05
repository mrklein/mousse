// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_vertex.hpp"
#include "mesh_tools.hpp"
#include "refine_cell.hpp"


// Static Functions 

// Update stored refine list using map
void mousse::edgeVertex::updateLabels
(
  const labelList& map,
  List<refineCell>& refCells
)
{
  label newRefI = 0;
  FOR_ALL(refCells, refI) {
    const refineCell& refCell = refCells[refI];
    label oldCellI = refCell.cellNo();
    label newCellI = map[oldCellI];
    if (newCellI != -1) {
      refCells[newRefI++] = refineCell(newCellI, refCell.direction());
    }
  }
  refCells.setSize(newRefI);
}


// Update stored cell numbers using map.
// Do in two passes to prevent allocation if nothing changed.
void mousse::edgeVertex::updateLabels
(
  const labelList& map,
  Map<label>& cellPairs
)
{
  // Iterate over map to see if anything changed
  bool changed = false;
  FOR_ALL_CONST_ITER(Map<label>, cellPairs, iter) {
    label newMaster = map[iter.key()];
    label newSlave = -1;
    if (iter() != -1) {
      newSlave = map[iter()];
    }
    if ((newMaster != iter.key()) || (newSlave != iter())) {
      changed = true;
      break;
    }
  }
  // Relabel (use second Map to prevent overlapping)
  if (changed) {
    Map<label> newCellPairs{2*cellPairs.size()};
    FOR_ALL_CONST_ITER(Map<label>, cellPairs, iter) {
      label newMaster = map[iter.key()];
      label newSlave = -1;
      if (iter() != -1) {
        newSlave = map[iter()];
      }
      if (newMaster == -1) {
        WARNING_IN
        (
          "edgeVertex::updateLabels(const labelList&, "
          "Map<label>&)"
        )
        << "master cell:" << iter.key()
        << " has disappeared" << endl;
      } else {
        newCellPairs.insert(newMaster, newSlave);
      }
    }
    cellPairs = newCellPairs;
  }
}


// Update stored cell numbers using map.
// Do in two passes to prevent allocation if nothing changed.
void mousse::edgeVertex::updateLabels
(
  const labelList& map,
  labelHashSet& cells
)
{
  // Iterate over map to see if anything changed
  bool changed = false;
  FOR_ALL_CONST_ITER(labelHashSet, cells, iter) {
    const label newCellI = map[iter.key()];
    if (newCellI != iter.key()) {
      changed = true;
      break;
    }
  }
  // Relabel (use second Map to prevent overlapping)
  if (changed) {
    labelHashSet newCells{2*cells.size()};
    FOR_ALL_CONST_ITER(labelHashSet, cells, iter) {
      const label newCellI = map[iter.key()];
      if (newCellI != -1) {
        newCells.insert(newCellI);
      }
    }
    cells = newCells;
  }
}


// Member Functions 
mousse::point mousse::edgeVertex::coord
(
  const primitiveMesh& mesh,
  const label cut,
  const scalar weight
)
{
  const pointField& pts = mesh.points();
  if (isEdge(mesh, cut)) {
    const edge& e = mesh.edges()[getEdge(mesh, cut)];
    return weight*pts[e.end()] + (1-weight)*pts[e.start()];
  } else {
    return pts[getVertex(mesh, cut)];
  }
}


mousse::label mousse::edgeVertex::cutPairToEdge
(
  const primitiveMesh& mesh,
  const label cut0,
  const label cut1
)
{
  if (!isEdge(mesh, cut0) && !isEdge(mesh, cut1)) {
    return
      meshTools::findEdge(mesh, getVertex(mesh, cut0), getVertex(mesh, cut1));
  } else {
    return -1;
  }
}


mousse::Ostream& mousse::edgeVertex::writeCut
(
  Ostream& os,
  const label cut,
  const scalar weight
) const
{
  if (isEdge(cut)) {
    label edgeI = getEdge(cut);
    const edge& e = mesh().edges()[edgeI];
    os << "edge:" << edgeI << e << ' ' << coord(cut, weight);
  } else {
    label vertI = getVertex(cut);
    os << "vertex:" << vertI << ' ' << coord(cut, weight);
  }
  return os;
}


mousse::Ostream& mousse::edgeVertex::writeCuts
(
  Ostream& os,
  const labelList& cuts,
  const scalarField& weights
) const
{
  FOR_ALL(cuts, cutI) {
    if (cutI > 0) {
      os << ' ';
    }
    writeCut(os, cuts[cutI], weights[cutI]);
  }
  return os;
}

