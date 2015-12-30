// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_mesh.hpp"
#include "fv_mesh_subset.hpp"
#include "time.hpp"
#include "cell_set.hpp"
// Constructors 
mousse::vtkMesh::vtkMesh
(
  fvMesh& baseMesh,
  const word& setName
)
:
  baseMesh_(baseMesh),
  subsetter_(baseMesh),
  setName_(setName)
{
  if (setName.size())
  {
    // Read cellSet using whole mesh
    cellSet currentSet(baseMesh_, setName_);
    // Set current subset
    subsetter_.setLargeCellSubset(currentSet);
  }
}
// Member Functions 
mousse::polyMesh::readUpdateState mousse::vtkMesh::readUpdate()
{
  polyMesh::readUpdateState meshState = baseMesh_.readUpdate();
  if (meshState != polyMesh::UNCHANGED)
  {
    // Note: since fvMeshSubset has no movePoints() functionality,
    // reconstruct the subset even if only movement.
    topoPtr_.clear();
    if (setName_.size())
    {
      Info<< "Subsetting mesh based on cellSet " << setName_ << endl;
      // Read cellSet using whole mesh
      cellSet currentSet(baseMesh_, setName_);
      subsetter_.setLargeCellSubset(currentSet);
    }
  }
  return meshState;
}
