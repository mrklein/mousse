// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_to_cell.hpp"
#include "poly_mesh.hpp"
#include "cell_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(cellToCell, 0);
addToRunTimeSelectionTable(topoSetSource, cellToCell, word);
addToRunTimeSelectionTable(topoSetSource, cellToCell, istream);
}
mousse::topoSetSource::addToUsageTable mousse::cellToCell::usage_
(
  cellToCell::typeName,
  "\n    Usage: cellToCell <cellSet>\n\n"
  "    Select all cells in the cellSet\n\n"
);
// Constructors 
// Construct from components
mousse::cellToCell::cellToCell
(
  const polyMesh& mesh,
  const word& setName
)
:
  topoSetSource(mesh),
  setName_(setName)
{}
// Construct from dictionary
mousse::cellToCell::cellToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set"))
{}
// Construct from Istream
mousse::cellToCell::cellToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is))
{}
// Destructor 
mousse::cellToCell::~cellToCell()
{}
// Member Functions 
void mousse::cellToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::ADD) || (action == topoSetSource::NEW))
  {
    Info<< "    Adding all elements of cellSet " << setName_ << " ..."
      << endl;
    // Load the set
    cellSet loadedSet(mesh_, setName_);
    set.addSet(loadedSet);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all elements of cellSet " << setName_ << " ..."
      << endl;
    // Load the set
    cellSet loadedSet(mesh_, setName_);
    set.deleteSet(loadedSet);
  }
}
