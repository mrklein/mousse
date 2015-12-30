// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "set_to_cell_zone.hpp"
#include "poly_mesh.hpp"
#include "cell_zone_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(setToCellZone, 0);
addToRunTimeSelectionTable(topoSetSource, setToCellZone, word);
addToRunTimeSelectionTable(topoSetSource, setToCellZone, istream);
}
mousse::topoSetSource::addToUsageTable mousse::setToCellZone::usage_
(
  setToCellZone::typeName,
  "\n    Usage: setToCellZone <cellSet>\n\n"
  "    Select all cells in the cellSet.\n\n"
);
// Constructors 
// Construct from components
mousse::setToCellZone::setToCellZone
(
  const polyMesh& mesh,
  const word& setName
)
:
  topoSetSource(mesh),
  setName_(setName)
{}
// Construct from dictionary
mousse::setToCellZone::setToCellZone
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set"))
{}
// Construct from Istream
mousse::setToCellZone::setToCellZone
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is))
{}
// Destructor 
mousse::setToCellZone::~setToCellZone()
{}
// Member Functions 
void mousse::setToCellZone::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if (!isA<cellZoneSet>(set))
  {
    WarningIn
    (
      "setToCellZone::applyToSet(const topoSetSource::setAction"
      ", topoSet"
    )   << "Operation only allowed on a cellZoneSet." << endl;
  }
  else
  {
    cellZoneSet& fzSet = refCast<cellZoneSet>(set);
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
      Info<< "    Adding all cells from cellSet " << setName_
        << " ..." << endl;
      // Load the sets
      cellSet fSet(mesh_, setName_);
      // Start off from copy
      DynamicList<label> newAddressing(fzSet.addressing());
      forAllConstIter(cellSet, fSet, iter)
      {
        label cellI = iter.key();
        if (!fzSet.found(cellI))
        {
          newAddressing.append(cellI);
        }
      }
      fzSet.addressing().transfer(newAddressing);
      fzSet.updateSet();
    }
    else if (action == topoSetSource::DELETE)
    {
      Info<< "    Removing all cells from cellSet " << setName_
        << " ..." << endl;
      // Load the set
      cellSet loadedSet(mesh_, setName_);
      // Start off empty
      DynamicList<label> newAddressing(fzSet.addressing().size());
      forAll(fzSet.addressing(), i)
      {
        if (!loadedSet.found(fzSet.addressing()[i]))
        {
          newAddressing.append(fzSet.addressing()[i]);
        }
      }
      fzSet.addressing().transfer(newAddressing);
      fzSet.updateSet();
    }
  }
}
