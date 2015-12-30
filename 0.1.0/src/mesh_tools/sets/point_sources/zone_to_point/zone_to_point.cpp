// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zone_to_point.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(zoneToPoint, 0);
addToRunTimeSelectionTable(topoSetSource, zoneToPoint, word);
addToRunTimeSelectionTable(topoSetSource, zoneToPoint, istream);
}
mousse::topoSetSource::addToUsageTable mousse::zoneToPoint::usage_
(
  zoneToPoint::typeName,
  "\n    Usage: zoneToPoint zone\n\n"
  "    Select all points in the pointZone."
  " Note:accepts wildcards for zone.\n\n"
);
// Private Member Functions 
void mousse::zoneToPoint::combine(topoSet& set, const bool add) const
{
  bool hasMatched = false;
  forAll(mesh_.pointZones(), i)
  {
    const pointZone& zone = mesh_.pointZones()[i];
    if (zoneName_.match(zone.name()))
    {
      const labelList& pointLabels = mesh_.pointZones()[i];
      Info<< "    Found matching zone " << zone.name()
        << " with " << pointLabels.size() << " points." << endl;
      hasMatched = true;
      forAll(pointLabels, i)
      {
        // Only do active points
        if (pointLabels[i] < mesh_.nPoints())
        {
          addOrDelete(set, pointLabels[i], add);
        }
      }
    }
  }
  if (!hasMatched)
  {
    WarningIn("zoneToPoint::combine(topoSet&, const bool)")
      << "Cannot find any pointZone named " << zoneName_ << endl
      << "Valid names are " << mesh_.pointZones().names() << endl;
  }
}
// Constructors 
// Construct from components
mousse::zoneToPoint::zoneToPoint
(
  const polyMesh& mesh,
  const word& zoneName
)
:
  topoSetSource(mesh),
  zoneName_(zoneName)
{}
// Construct from dictionary
mousse::zoneToPoint::zoneToPoint
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  zoneName_(dict.lookup("name"))
{}
// Construct from Istream
mousse::zoneToPoint::zoneToPoint
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  zoneName_(checkIs(is))
{}
// Destructor 
mousse::zoneToPoint::~zoneToPoint()
{}
// Member Functions 
void mousse::zoneToPoint::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all points of pointZone " << zoneName_ << " ..."
      << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all points of pointZone " << zoneName_ << " ..."
      << endl;
    combine(set, false);
  }
}
