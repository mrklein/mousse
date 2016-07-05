// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "near_wall_dist.hpp"
#include "fv_mesh.hpp"
#include "cell_dist_funcs.hpp"
#include "wall_fv_patch.hpp"
#include "surface_fields.hpp"
// Private Member Functions 
void mousse::nearWallDist::calculate()
{
  cellDistFuncs wallUtils(mesh_);
  // Get patch ids of walls
  labelHashSet wallPatchIDs(wallUtils.getPatchIDs<wallPolyPatch>());
  // Size neighbours array for maximum possible
  labelList neighbours(wallUtils.maxPatchSize(wallPatchIDs));
  // Correct all cells with face on wall
  const volVectorField& cellCentres = mesh_.C();
  FOR_ALL(mesh_.boundary(), patchI)
  {
    fvPatchScalarField& ypatch = operator[](patchI);
    const fvPatch& patch = mesh_.boundary()[patchI];
    if (isA<wallFvPatch>(patch))
    {
      const polyPatch& pPatch = patch.patch();
      const labelUList& faceCells = patch.faceCells();
      // Check cells with face on wall
      FOR_ALL(patch, patchFaceI)
      {
        label nNeighbours = wallUtils.getPointNeighbours
        (
          pPatch,
          patchFaceI,
          neighbours
        );
        label minFaceI = -1;
        ypatch[patchFaceI] = wallUtils.smallestDist
        (
          cellCentres[faceCells[patchFaceI]],
          pPatch,
          nNeighbours,
          neighbours,
          minFaceI
        );
      }
    }
    else
    {
      ypatch = 0.0;
    }
  }
}
// Constructors 
mousse::nearWallDist::nearWallDist(const mousse::fvMesh& mesh)
:
  volScalarField::GeometricBoundaryField
  (
    mesh.boundary(),
    mesh.V(),           // Dummy internal field,
    calculatedFvPatchScalarField::typeName
  ),
  mesh_(mesh)
{
  calculate();
}
// Destructor 
mousse::nearWallDist::~nearWallDist()
{}
// Member Functions 
void mousse::nearWallDist::correct()
{
  if (mesh_.topoChanging())
  {
    const DimensionedField<scalar, volMesh>& V = mesh_.V();
    const fvBoundaryMesh& bnd = mesh_.boundary();
    this->setSize(bnd.size());
    FOR_ALL(*this, patchI)
    {
      this->set
      (
        patchI,
        fvPatchField<scalar>::New
        (
          calculatedFvPatchScalarField::typeName,
          bnd[patchI],
          V
        )
      );
    }
  }
  calculate();
}
