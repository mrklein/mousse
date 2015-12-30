// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "set_to_point_zone.hpp"
#include "poly_mesh.hpp"
#include "point_zone_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(setToPointZone, 0);
addToRunTimeSelectionTable(topoSetSource, setToPointZone, word);
addToRunTimeSelectionTable(topoSetSource, setToPointZone, istream);
}
mousse::topoSetSource::addToUsageTable mousse::setToPointZone::usage_
(
  setToPointZone::typeName,
  "\n    Usage: setToPointZone <pointSet>\n\n"
  "    Select all points in the pointSet.\n\n"
);
// Constructors 
// Construct from components
mousse::setToPointZone::setToPointZone
(
  const polyMesh& mesh,
  const word& setName
)
:
  topoSetSource(mesh),
  setName_(setName)
{}
// Construct from dictionary
mousse::setToPointZone::setToPointZone
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set"))
{}
// Construct from Istream
mousse::setToPointZone::setToPointZone
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is))
{}
// Destructor 
mousse::setToPointZone::~setToPointZone()
{}
// Member Functions 
void mousse::setToPointZone::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if (!isA<pointZoneSet>(set))
  {
    WarningIn
    (
      "setToPointZone::applyToSet(const topoSetSource::setAction"
      ", topoSet"
    )   << "Operation only allowed on a pointZoneSet." << endl;
  }
  else
  {
    pointZoneSet& fzSet = refCast<pointZoneSet>(set);
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
      Info<< "    Adding all points from pointSet " << setName_
        << " ..." << endl;
      // Load the sets
      pointSet fSet(mesh_, setName_);
      // Start off from copy
      DynamicList<label> newAddressing(fzSet.addressing());
      forAllConstIter(pointSet, fSet, iter)
      {
        label pointI = iter.key();
        if (!fzSet.found(pointI))
        {
          newAddressing.append(pointI);
        }
      }
      fzSet.addressing().transfer(newAddressing);
      fzSet.updateSet();
    }
    else if (action == topoSetSource::DELETE)
    {
      Info<< "    Removing all points from pointSet " << setName_
        << " ..." << endl;
      // Load the set
      pointSet loadedSet(mesh_, setName_);
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
