// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sets_to_face_zone.hpp"
#include "poly_mesh.hpp"
#include "face_zone_set.hpp"
#include "cell_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(setsToFaceZone, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, setsToFaceZone, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, setsToFaceZone, istream);
}

mousse::topoSetSource::addToUsageTable mousse::setsToFaceZone::usage_
(
  setsToFaceZone::typeName,
  "\n    Usage: setsToFaceZone <faceSet> <slaveCellSet>\n\n"
  "    Select all faces in the faceSet."
  " Orientated so slave side is in cellSet.\n\n"
);

// Constructors 
// Construct from components
mousse::setsToFaceZone::setsToFaceZone
(
  const polyMesh& mesh,
  const word& faceSetName,
  const word& cellSetName,
  const Switch& flip
)
:
  topoSetSource{mesh},
  faceSetName_{faceSetName},
  cellSetName_{cellSetName},
  flip_{flip}
{}
// Construct from dictionary
mousse::setsToFaceZone::setsToFaceZone
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  faceSetName_{dict.lookup("faceSet")},
  cellSetName_{dict.lookup("cellSet")},
  flip_{dict.lookupOrDefault("flip", false)}
{}
// Construct from Istream
mousse::setsToFaceZone::setsToFaceZone
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  faceSetName_(checkIs(is)),
  cellSetName_(checkIs(is)),
  flip_(false)
{}
// Destructor 
mousse::setsToFaceZone::~setsToFaceZone()
{}
// Member Functions 
void mousse::setsToFaceZone::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if (!isA<faceZoneSet>(set))
  {
    WARNING_IN
    (
      "setsToFaceZone::applyToSet(const topoSetSource::setAction"
      ", topoSet"
    )   << "Operation only allowed on a faceZoneSet." << endl;
  }
  else
  {
    faceZoneSet& fzSet = refCast<faceZoneSet>(set);
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
      Info<< "    Adding all faces from faceSet " << faceSetName_
        << " ..." << endl;
      // Load the sets
      faceSet fSet(mesh_, faceSetName_);
      cellSet cSet(mesh_, cellSetName_);
      // Start off from copy
      DynamicList<label> newAddressing(fzSet.addressing());
      DynamicList<bool> newFlipMap(fzSet.flipMap());
      FOR_ALL_CONST_ITER(faceSet, fSet, iter)
      {
        label faceI = iter.key();
        if (!fzSet.found(faceI))
        {
          bool flipFace = false;
          label own = mesh_.faceOwner()[faceI];
          bool ownFound = cSet.found(own);
          if (mesh_.isInternalFace(faceI))
          {
            label nei = mesh_.faceNeighbour()[faceI];
            bool neiFound = cSet.found(nei);
            if (ownFound && !neiFound)
            {
              flipFace = false;
            }
            else if (!ownFound && neiFound)
            {
              flipFace = true;
            }
            else
            {
              WARNING_IN
              (
                "setsToFaceZone::applyToSet"
                "(const topoSetSource::setAction, topoSet)"
              )
              << "One of owner or neighbour of internal face "
              << faceI << " should be in cellSet "
              << cSet.name()
              << " to be able to determine orientation."
              << endl
              << "Face:" << faceI << " own:" << own
              << " OwnInCellSet:" << ownFound
              << " nei:" << nei
              << " NeiInCellSet:" << neiFound
              << endl;
            }
          }
          else
          {
            flipFace = !ownFound;
          }
          if (flip_)
          {
            flipFace = !flipFace;
          }
          newAddressing.append(faceI);
          newFlipMap.append(flipFace);
        }
      }
      fzSet.addressing().transfer(newAddressing);
      fzSet.flipMap().transfer(newFlipMap);
      fzSet.updateSet();
    }
    else if (action == topoSetSource::DELETE)
    {
      Info<< "    Removing all faces from faceSet " << faceSetName_
        << " ..." << endl;
      // Load the set
      faceZoneSet loadedSet(mesh_, faceSetName_);
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
