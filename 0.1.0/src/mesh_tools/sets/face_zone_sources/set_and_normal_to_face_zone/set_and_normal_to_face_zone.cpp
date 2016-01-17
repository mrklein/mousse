// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "set_and_normal_to_face_zone.hpp"
#include "poly_mesh.hpp"
#include "face_zone_set.hpp"
#include "add_to_run_time_selection_table.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(setAndNormalToFaceZone, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, setAndNormalToFaceZone, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, setAndNormalToFaceZone, istream);
}

mousse::topoSetSource::addToUsageTable mousse::setAndNormalToFaceZone::usage_
(
  setAndNormalToFaceZone::typeName,
  "\n    Usage: setAndNormalToFaceZone <faceSet> <normal>\n\n"
  "    Select all faces in the faceSet and orient using normal.\n\n"
);

// Constructors 
mousse::setAndNormalToFaceZone::setAndNormalToFaceZone
(
  const polyMesh& mesh,
  const word& setName,
  const vector& normal
)
:
  topoSetSource(mesh),
  setName_(setName),
  normal_(normal)
{}
mousse::setAndNormalToFaceZone::setAndNormalToFaceZone
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("faceSet")),
  normal_(dict.lookup("normal"))
{}
mousse::setAndNormalToFaceZone::setAndNormalToFaceZone
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is)),
  normal_(checkIs(is))
{}
// Destructor 
mousse::setAndNormalToFaceZone::~setAndNormalToFaceZone()
{}
// Member Functions 
void mousse::setAndNormalToFaceZone::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if (!isA<faceZoneSet>(set))
  {
    WARNING_IN
    (
      "setAndNormalToFaceZone::applyToSet"
      "("
        "const topoSetSource::setAction, "
        "topoSet&"
      ")"
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
      const faceList& faces = mesh_.faces();
      const pointField& points = mesh_.points();
      FOR_ALL_CONST_ITER(faceSet, fSet, iter)
      {
        label faceI = iter.key();
        if (!fzSet.found(faceI))
        {
          newAddressing.append(faceI);
          vector n = faces[faceI].normal(points);
          if ((n & normal_) > 0)
          {
            newFlipMap.append(false);
          }
          else
          {
            newFlipMap.append(true);
          }
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
