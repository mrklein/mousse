// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_surface_selection.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "sync_tools.hpp"
#include "searchable_surface.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {
namespace faceSelections {

DEFINE_TYPE_NAME_AND_DEBUG(searchableSurfaceSelection, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  faceSelection,
  searchableSurfaceSelection,
  dictionary
);

}
}


// Constructors 
mousse::faceSelections::searchableSurfaceSelection::searchableSurfaceSelection
(
  const word& name,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  faceSelection{name, mesh, dict},
  surfacePtr_
  {
    searchableSurface::New
    (
      word{dict.lookup("surface")},
      {
        dict.lookupOrDefault("name", mesh.objectRegistry::db().name()),
        mesh.time().constant(),
        "triSurface",
        mesh.objectRegistry::db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      },
      dict
    )
  }
{}


// Destructor 
mousse::faceSelections::searchableSurfaceSelection::~searchableSurfaceSelection()
{}


// Member Functions 
void mousse::faceSelections::searchableSurfaceSelection::select
(
  const label zoneID,
  labelList& faceToZoneID,
  boolList& faceToFlip
) const
{
  // Get cell-cell centre vectors
  pointField start{mesh_.nFaces()};
  pointField end{mesh_.nFaces()};
  // Internal faces
  for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++) {
    start[faceI] = mesh_.cellCentres()[mesh_.faceOwner()[faceI]];
    end[faceI] = mesh_.cellCentres()[mesh_.faceNeighbour()[faceI]];
  }
  // Boundary faces
  vectorField neighbourCellCentres;
  syncTools::swapBoundaryCellPositions
  (
    mesh_,
    mesh_.cellCentres(),
    neighbourCellCentres
  );
  const polyBoundaryMesh& pbm = mesh_.boundaryMesh();
  FOR_ALL(pbm, patchI) {
    const polyPatch& pp = pbm[patchI];
    if (pp.coupled()) {
      FOR_ALL(pp, i) {
        label faceI = pp.start()+i;
        start[faceI] = mesh_.cellCentres()[mesh_.faceOwner()[faceI]];
        end[faceI] = neighbourCellCentres[faceI-mesh_.nInternalFaces()];
      }
    } else {
      FOR_ALL(pp, i) {
        label faceI = pp.start()+i;
        start[faceI] = mesh_.cellCentres()[mesh_.faceOwner()[faceI]];
        end[faceI] = mesh_.faceCentres()[faceI];
      }
    }
  }
  List<pointIndexHit> hits;
  surfacePtr_().findLine(start, end, hits);
  pointField normals;
  surfacePtr_().getNormal(hits, normals);
  //- Note: do not select boundary faces.
  for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++) {
    if (hits[faceI].hit()) {
      faceToZoneID[faceI] = zoneID;
      vector d = end[faceI]-start[faceI];
      faceToFlip[faceI] = ((normals[faceI] & d) < 0);
    }
  }
  FOR_ALL(pbm, patchI) {
    const polyPatch& pp = pbm[patchI];
    if (pp.coupled()) {
      FOR_ALL(pp, i) {
        label faceI = pp.start()+i;
        if (hits[faceI].hit()) {
          faceToZoneID[faceI] = zoneID;
          vector d = end[faceI]-start[faceI];
          faceToFlip[faceI] = ((normals[faceI] & d) < 0);
        }
      }
    }
  }
  faceSelection::select(zoneID, faceToZoneID, faceToFlip);
}

