// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nearest_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(nearestToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, nearestToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, nearestToCell, istream);

}

mousse::topoSetSource::addToUsageTable mousse::nearestToCell::usage_
{
  nearestToCell::typeName,
  "\n    Usage: nearestToCell (pt0 .. ptn)\n\n"
  "    Select the nearest cell for each of the points pt0 ..ptn\n\n"
};


// Private Member Functions 
void mousse::nearestToCell::combine(topoSet& set, const bool add) const
{
  FOR_ALL(points_, pointI) {
    addOrDelete(set, mesh_.findNearestCell(points_[pointI]), add);
  }
}


// Constructors 

// Construct from components
mousse::nearestToCell::nearestToCell
(
  const polyMesh& mesh,
  const pointField& points
)
:
  topoSetSource{mesh},
  points_{points}
{}


// Construct from dictionary
mousse::nearestToCell::nearestToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  points_{dict.lookup("points")}
{}


// Construct from Istream
mousse::nearestToCell::nearestToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource{mesh},
  points_{checkIs(is)}
{}


// Destructor 
mousse::nearestToCell::~nearestToCell()
{}


// Member Functions 
void mousse::nearestToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD)) {
    Info << "    Adding cells nearest to " << points_ << endl;
    combine(set, true);
  } else if (action == topoSetSource::DELETE) {
    Info << "    Removing cells nearest to " << points_ << endl;
    combine(set, false);
  }
}

