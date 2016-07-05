// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "time.hpp"
#include "global_mesh_data.hpp"
#include "point_mesh.hpp"
#include "indexed_octree.hpp"
#include "tree_data_cell.hpp"


// Member Functions 
void mousse::polyMesh::updateMesh(const mapPolyMesh& mpm)
{
  if (debug) {
    Info << "void polyMesh::updateMesh(const mapPolyMesh&) : "
      << "updating addressing and (optional) pointMesh/pointFields"
      << endl;
  }
  // Update boundaryMesh (note that patches themselves already ok)
  boundary_.updateMesh();
  // Update zones
  pointZones_.clearAddressing();
  faceZones_.clearAddressing();
  cellZones_.clearAddressing();
  // Remove the stored tet base points
  tetBasePtIsPtr_.clear();
  // Remove the cell tree
  cellTreePtr_.clear();
  // Update parallel data
  if (globalMeshDataPtr_.valid()) {
    globalMeshDataPtr_->updateMesh();
  }
  setInstance(time().timeName());
  // Map the old motion points if present
  if (oldPointsPtr_.valid()) {
    // Make a copy of the original points
    pointField oldMotionPoints = oldPointsPtr_();
    pointField& newMotionPoints = oldPointsPtr_();
    // Resize the list to new size
    newMotionPoints.setSize(points_.size());
    // Map the list
    newMotionPoints.map(oldMotionPoints, mpm.pointMap());
    // Any points created out-of-nothing get set to the current coordinate
    // for lack of anything better.
    FOR_ALL(mpm.pointMap(), newPointI) {
      if (mpm.pointMap()[newPointI] == -1) {
        newMotionPoints[newPointI] = points_[newPointI];
      }
    }
  }
  meshObject::updateMesh<polyMesh>(*this, mpm);
  meshObject::updateMesh<pointMesh>(*this, mpm);
  // Reset valid directions (could change by faces put into empty patches)
  geometricD_ = Vector<label>::zero;
  solutionD_ = Vector<label>::zero;
  const_cast<Time&>(time()).functionObjects().updateMesh(mpm);
}

