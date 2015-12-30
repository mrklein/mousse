// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surf_mesh.hpp"
#include "global_mesh_data.hpp"
#include "demand_driven_data.hpp"
// Private Member Functions 
void mousse::surfMesh::removeZones()
{
  if (debug)
  {
    Info<< "void surfMesh::removeZones(): "
      << "Removing surface zones."
      << endl;
  }
  // Remove the surface zones
  storedZones().clear();
  clearOut();
}
// Member Functions 
void mousse::surfMesh::clearGeom()
{
  if (debug)
  {
    Info<< "void surfMesh::clearGeom() : "
      << "clearing geometric data"
      << endl;
  }
  MeshReference::clearGeom();
}
void mousse::surfMesh::clearAddressing()
{
  if (debug)
  {
    Info<< "void surfMesh::clearAddressing() : "
      << "clearing topology"
      << endl;
  }
  MeshReference::clearPatchMeshAddr();
}
void mousse::surfMesh::clearOut()
{
  clearGeom();
  clearAddressing();
}
