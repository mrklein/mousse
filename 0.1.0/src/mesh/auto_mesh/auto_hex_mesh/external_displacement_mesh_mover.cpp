// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "external_displacement_mesh_mover.hpp"
#include "map_poly_mesh.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(externalDisplacementMeshMover, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(externalDisplacementMeshMover, dictionary);

}


// Constructors 
mousse::externalDisplacementMeshMover::externalDisplacementMeshMover
(
  const dictionary&,
  const List<labelPair>& baffles,
  pointVectorField& pointDisplacement
)
:
  baffles_{baffles},
  pointDisplacement_{pointDisplacement}
{}


// Selectors
mousse::autoPtr<mousse::externalDisplacementMeshMover>
mousse::externalDisplacementMeshMover::New
(
  const word& type,
  const dictionary& dict,
  const List<labelPair>& baffles,
  pointVectorField& pointDisplacement
)
{
  Info << "Selecting externalDisplacementMeshMover " << type << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(type);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "externalDisplacementMeshMover::New(const word&"
      ", pointVectorField&, const List<labelPair>&"
      ", const dictionary&)"
    )
    << "Unknown externalDisplacementMeshMover type "
    << type << nl << nl
    << "Valid externalDisplacementMeshMover types:" << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return
    autoPtr<externalDisplacementMeshMover>
    {
      cstrIter()(dict, baffles, pointDisplacement)
    };
}


// Destructor 
mousse::externalDisplacementMeshMover::~externalDisplacementMeshMover()
{}


// Member Functions 
void mousse::externalDisplacementMeshMover::movePoints(const pointField&)
{
  // No local data to update
}


void mousse::externalDisplacementMeshMover::updateMesh(const mapPolyMesh& mpm)
{
  // Renumber baffles
  DynamicList<labelPair> newBaffles{baffles_.size()};
  FOR_ALL(baffles_, i) {
    label f0 = mpm.reverseFaceMap()[baffles_[i].first()];
    label f1 = mpm.reverseFaceMap()[baffles_[i].second()];
    if (f0 >= 0 && f1 >= 0) {
      newBaffles.append(labelPair(f0, f1));
    }
  }
  newBaffles.shrink();
  baffles_.transfer(newBaffles);
}

