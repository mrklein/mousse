// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_surface_to_face_zone.hpp"
#include "poly_mesh.hpp"
#include "face_zone_set.hpp"
#include "searchable_surface.hpp"
#include "sync_tools.hpp"
#include "time.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(searchableSurfaceToFaceZone, 0);
  addToRunTimeSelectionTable
  (
    topoSetSource,
    searchableSurfaceToFaceZone,
    word
  );
}
mousse::topoSetSource::addToUsageTable mousse::searchableSurfaceToFaceZone::usage_
(
  searchableSurfaceToFaceZone::typeName,
  "\n    Usage: searchableSurfaceToFaceZone surface\n\n"
  "    Select all faces whose cell-cell centre vector intersects the surface "
  "\n"
);
// Constructors 
// Construct from dictionary
mousse::searchableSurfaceToFaceZone::searchableSurfaceToFaceZone
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  surfacePtr_
  (
    searchableSurface::New
    (
      word(dict.lookup("surface")),
      IOobject
      (
        dict.lookupOrDefault("name", mesh.objectRegistry::db().name()),
        mesh.time().constant(),
        "triSurface",
        mesh.objectRegistry::db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      dict
    )
  )
{}
// Destructor 
mousse::searchableSurfaceToFaceZone::~searchableSurfaceToFaceZone()
{}
// Member Functions 
void mousse::searchableSurfaceToFaceZone::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if (!isA<faceZoneSet>(set))
  {
    WarningIn
    (
      "searchableSurfaceToFaceZone::applyToSet"
      "(const topoSetSource::setAction"
      ", topoSet"
    )   << "Operation only allowed on a faceZoneSet." << endl;
  }
  else
  {
    faceZoneSet& fzSet = refCast<faceZoneSet>(set);
    // Get cell-cell centre vectors
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    pointField start(mesh_.nFaces());
    pointField end(mesh_.nFaces());
    const pointField& cc = mesh_.cellCentres();
    // Internal faces
    for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++)
    {
      start[faceI] = cc[mesh_.faceOwner()[faceI]];
      end[faceI] = cc[mesh_.faceNeighbour()[faceI]];
    }
    // Boundary faces
    vectorField nbrCellCentres;
    syncTools::swapBoundaryCellPositions(mesh_, cc, nbrCellCentres);
    const polyBoundaryMesh& pbm = mesh_.boundaryMesh();
    forAll(pbm, patchI)
    {
      const polyPatch& pp = pbm[patchI];
      if (pp.coupled())
      {
        forAll(pp, i)
        {
          label faceI = pp.start()+i;
          start[faceI] = cc[mesh_.faceOwner()[faceI]];
          end[faceI] = nbrCellCentres[faceI-mesh_.nInternalFaces()];
        }
      }
      else
      {
        forAll(pp, i)
        {
          label faceI = pp.start()+i;
          start[faceI] = cc[mesh_.faceOwner()[faceI]];
          end[faceI] = mesh_.faceCentres()[faceI];
        }
      }
    }
    // Do all intersection tests
    // ~~~~~~~~~~~~~~~~~~~~~~~~~
    List<pointIndexHit> hits;
    surfacePtr_().findLine(start, end, hits);
    pointField normals;
    surfacePtr_().getNormal(hits, normals);
    // Select intersected faces
    // ~~~~~~~~~~~~~~~~~~~~~~~~
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
      Info<< "    Adding all faces from surface "
        << surfacePtr_().name() << " ..." << endl;
      DynamicList<label> newAddressing(fzSet.addressing());
      DynamicList<bool> newFlipMap(fzSet.flipMap());
      forAll(hits, faceI)
      {
        if (hits[faceI].hit() && !fzSet.found(faceI))
        {
          newAddressing.append(faceI);
          vector d = end[faceI]-start[faceI];
          newFlipMap.append((normals[faceI] & d) < 0);
        }
      }
      fzSet.addressing().transfer(newAddressing);
      fzSet.flipMap().transfer(newFlipMap);
      fzSet.updateSet();
    }
    else if (action == topoSetSource::DELETE)
    {
      Info<< "    Removing all faces from surface "
        << surfacePtr_().name() << " ..." << endl;
      // Start off empty
      DynamicList<label> newAddressing(fzSet.addressing().size());
      DynamicList<bool> newFlipMap(fzSet.flipMap().size());
      forAll(fzSet.addressing(), i)
      {
        if (!hits[fzSet.addressing()[i]].hit())
        {
          newAddressing.append(fzSet.addressing()[i]);
          newFlipMap.append(fzSet.flipMap()[i]);
        }
      }
      fzSet.addressing().transfer(newAddressing);
      fzSet.flipMap().transfer(newFlipMap);
      fzSet.updateSet();
    }
  }
}
