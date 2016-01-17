// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "set_to_face_zone.hpp"
#include "poly_mesh.hpp"
#include "face_zone_set.hpp"
#include "add_to_run_time_selection_table.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(setToFaceZone, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, setToFaceZone, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, setToFaceZone, istream);
}

mousse::topoSetSource::addToUsageTable mousse::setToFaceZone::usage_
(
  setToFaceZone::typeName,
  "\n    Usage: setToFaceZone <faceSet>\n\n"
  "    Select all faces in the faceSet."
  " Sets flipMap.\n\n"
);

// Constructors 
// Construct from components
mousse::setToFaceZone::setToFaceZone
(
  const polyMesh& mesh,
  const word& setName
)
:
  topoSetSource(mesh),
  setName_(setName)
{}
// Construct from dictionary
mousse::setToFaceZone::setToFaceZone
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("faceSet"))
{}
// Construct from Istream
mousse::setToFaceZone::setToFaceZone
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is))
{}
// Destructor 
mousse::setToFaceZone::~setToFaceZone()
{}
// Member Functions 
void mousse::setToFaceZone::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if (!isA<faceZoneSet>(set))
  {
    WARNING_IN
    (
      "setToFaceZone::applyToSet(const topoSetSource::setAction"
      ", topoSet"
    )   << "Operation only allowed on a faceZoneSet." << endl;
  }
  else
  {
    faceZoneSet& fzSet = refCast<faceZoneSet>(set);
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
      Info<< "    Adding all faces from faceSet " << setName_
        << " ..." << endl;
      // Load the sets
      faceSet fSet(mesh_, setName_);
      // Start off from copy
      DynamicList<label> newAddressing(fzSet.addressing());
      DynamicList<bool> newFlipMap(fzSet.flipMap());
      FOR_ALL_CONST_ITER(faceSet, fSet, iter)
      {
        label faceI = iter.key();
        if (!fzSet.found(faceI))
        {
          newAddressing.append(faceI);
          newFlipMap.append(false);
        }
      }
      fzSet.addressing().transfer(newAddressing);
      fzSet.flipMap().transfer(newFlipMap);
      fzSet.updateSet();
    }
    else if (action == topoSetSource::DELETE)
    {
      Info<< "    Removing all faces from faceSet " << setName_
        << " ..." << endl;
      // Load the set
      faceSet loadedSet(mesh_, setName_);
      // Start off empty
      DynamicList<label> newAddressing(fzSet.addressing().size());
      DynamicList<bool> newFlipMap(fzSet.flipMap().size());
      FOR_ALL(fzSet.addressing(), i)
      {
        if (!loadedSet.found(fzSet.addressing()[i]))
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
