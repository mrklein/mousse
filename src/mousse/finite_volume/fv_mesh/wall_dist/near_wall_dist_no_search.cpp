// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "near_wall_dist_no_search.hpp"
#include "fv_mesh.hpp"
#include "wall_fv_patch.hpp"
#include "surface_fields.hpp"
// Private Member Functions 
void mousse::nearWallDistNoSearch::doAll()
{
  const volVectorField& cellCentres = mesh_.C();
  const fvPatchList& patches = mesh_.boundary();
  FOR_ALL(patches, patchI)
  {
    fvPatchScalarField& ypatch = operator[](patchI);
    if (isA<wallFvPatch>(patches[patchI]))
    {
      const labelUList& faceCells = patches[patchI].faceCells();
      const fvPatchVectorField& patchCentres
        = cellCentres.boundaryField()[patchI];
      const fvsPatchVectorField& Apatch
        = mesh_.Sf().boundaryField()[patchI];
      const fvsPatchScalarField& magApatch
        = mesh_.magSf().boundaryField()[patchI];
      FOR_ALL(patchCentres, facei)
      {
        ypatch[facei] =
        (
          Apatch[facei] &
          (
            patchCentres[facei]
           - cellCentres[faceCells[facei]]
          )
        )/magApatch[facei];
      }
    }
    else
    {
      ypatch = 0.0;
    }
  }
}
// Constructors 
mousse::nearWallDistNoSearch::nearWallDistNoSearch(const mousse::fvMesh& mesh)
:
  volScalarField::GeometricBoundaryField
  (
    mesh.boundary(),
    mesh.V(),           // Dummy internal field
    calculatedFvPatchScalarField::typeName
  ),
  mesh_(mesh)
{
  doAll();
}
// Destructor 
mousse::nearWallDistNoSearch::~nearWallDistNoSearch()
{}
// Member Functions 
void mousse::nearWallDistNoSearch::correct()
{
  if (mesh_.changing())
  {
    // Update size of GeometricBoundaryField
    FOR_ALL(mesh_.boundary(), patchI)
    {
      operator[](patchI).setSize(mesh_.boundary()[patchI].size());
    }
  }
  doAll();
}
