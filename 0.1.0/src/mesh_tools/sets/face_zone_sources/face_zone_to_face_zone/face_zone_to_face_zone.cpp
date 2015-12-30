// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_zone_to_face_zone.hpp"
#include "poly_mesh.hpp"
#include "face_zone_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(faceZoneToFaceZone, 0);
addToRunTimeSelectionTable(topoSetSource, faceZoneToFaceZone, word);
addToRunTimeSelectionTable(topoSetSource, faceZoneToFaceZone, istream);
}
mousse::topoSetSource::addToUsageTable mousse::faceZoneToFaceZone::usage_
(
  faceZoneToFaceZone::typeName,
  "\n    Usage: faceZoneToFaceZone <faceZone>\n\n"
  "    Select all faces in the faceZone\n\n"
);
// Constructors 
// Construct from components
mousse::faceZoneToFaceZone::faceZoneToFaceZone
(
  const polyMesh& mesh,
  const word& setName
)
:
  topoSetSource(mesh),
  setName_(setName)
{}
// Construct from dictionary
mousse::faceZoneToFaceZone::faceZoneToFaceZone
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("zone"))
{}
// Construct from Istream
mousse::faceZoneToFaceZone::faceZoneToFaceZone
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is))
{}
// Destructor 
mousse::faceZoneToFaceZone::~faceZoneToFaceZone()
{}
// Member Functions 
void mousse::faceZoneToFaceZone::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if (!isA<faceZoneSet>(set))
  {
    WarningIn
    (
      "faceZoneToFaceZone::applyToSet(const topoSetSource::setAction"
      ", topoSet"
    )   << "Operation only allowed on a faceZoneSet." << endl;
  }
  else
  {
    faceZoneSet& fSet = refCast<faceZoneSet>(set);
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
      Info<< "    Adding all faces from faceZone " << setName_ << " ..."
        << endl;
      // Load the set
      faceZoneSet loadedSet(mesh_, setName_);
      DynamicList<label> newAddressing(fSet.addressing());
      DynamicList<bool> newFlipMap(fSet.flipMap());
      forAll(loadedSet.addressing(), i)
      {
        if (!fSet.found(loadedSet.addressing()[i]))
        {
          newAddressing.append(loadedSet.addressing()[i]);
          newFlipMap.append(loadedSet.flipMap()[i]);
        }
      }
      fSet.addressing().transfer(newAddressing);
      fSet.flipMap().transfer(newFlipMap);
      fSet.updateSet();
    }
    else if (action == topoSetSource::DELETE)
    {
      Info<< "    Removing all faces from faceZone " << setName_ << " ..."
        << endl;
      // Load the set
      faceZoneSet loadedSet(mesh_, setName_);
      DynamicList<label> newAddressing(fSet.addressing().size());
      DynamicList<bool> newFlipMap(fSet.flipMap().size());
      forAll(fSet.addressing(), i)
      {
        if (!loadedSet.found(fSet.addressing()[i]))
        {
          newAddressing.append(fSet.addressing()[i]);
          newFlipMap.append(fSet.flipMap()[i]);
        }
      }
      fSet.addressing().transfer(newAddressing);
      fSet.flipMap().transfer(newFlipMap);
      fSet.updateSet();
    }
  }
}
