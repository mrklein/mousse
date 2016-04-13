// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "global_mesh_data.hpp"
#include "_mesh_object.hpp"
#include "indexed_octree.hpp"
#include "tree_data_cell.hpp"
#include "point_mesh.hpp"


// Private Member Functions 
void mousse::polyMesh::removeBoundary()
{
  if (debug) {
    Info << "void polyMesh::removeBoundary(): "
      << "Removing boundary patches."
      << endl;
  }
  // Remove the point zones
  boundary_.clear();
  boundary_.setSize(0);
  clearOut();
}


// Member Functions 
void mousse::polyMesh::clearGeom()
{
  if (debug) {
    Info << "void polyMesh::clearGeom() : "
      << "clearing geometric data"
      << endl;
  }
  // Clear all geometric mesh objects
  meshObject::clear<pointMesh, GeometricMeshObject>(*this);
  meshObject::clear<polyMesh, GeometricMeshObject>(*this);
  primitiveMesh::clearGeom();
  boundary_.clearGeom();
  // Reset valid directions (could change with rotation)
  geometricD_ = Vector<label>::zero;
  solutionD_ = Vector<label>::zero;
  // Remove the stored tet base points
  tetBasePtIsPtr_.clear();
  // Remove the cell tree
  cellTreePtr_.clear();
}


void mousse::polyMesh::clearAdditionalGeom()
{
  if (debug) {
    Info << "void polyMesh::clearAdditionalGeom() : "
      << "clearing additional geometric data"
      << endl;
  }
  // Remove the stored tet base points
  tetBasePtIsPtr_.clear();
  // Remove the cell tree
  cellTreePtr_.clear();
}


void mousse::polyMesh::clearAddressing(const bool isMeshUpdate)
{
  if (debug) {
    Info << "void polyMesh::clearAddressing() : "
      << "clearing topology  isMeshUpdate:" << isMeshUpdate
      << endl;
  }
  if (isMeshUpdate) {
    // Part of a mesh update. Keep meshObjects that have an updateMesh
    // callback
    meshObject::clearUpto
    <
      pointMesh,
      TopologicalMeshObject,
      UpdateableMeshObject
    >
    (
      *this
    );
    meshObject::clearUpto
    <
      polyMesh,
      TopologicalMeshObject,
      UpdateableMeshObject
    >
    (
      *this
    );
  } else {
    meshObject::clear<pointMesh, TopologicalMeshObject>(*this);
    meshObject::clear<polyMesh, TopologicalMeshObject>(*this);
  }
  primitiveMesh::clearAddressing();
  // parallelData depends on the processorPatch ordering so force
  // recalculation
  globalMeshDataPtr_.clear();
  // Reset valid directions
  geometricD_ = Vector<label>::zero;
  solutionD_ = Vector<label>::zero;
  // Update zones
  pointZones_.clearAddressing();
  faceZones_.clearAddressing();
  cellZones_.clearAddressing();
  // Remove the stored tet base points
  tetBasePtIsPtr_.clear();
  // Remove the cell tree
  cellTreePtr_.clear();
}


void mousse::polyMesh::clearPrimitives()
{
  resetMotion();
  points_.setSize(0);
  faces_.setSize(0);
  owner_.setSize(0);
  neighbour_.setSize(0);
  clearedPrimitives_ = true;
}


void mousse::polyMesh::clearOut()
{
  clearGeom();
  clearAddressing();
}


void mousse::polyMesh::clearCellTree()
{
  if (debug) {
    Info << "void polyMesh::clearCellTree() : "
      << "clearing cell tree"
      << endl;
  }
  cellTreePtr_.clear();
}

