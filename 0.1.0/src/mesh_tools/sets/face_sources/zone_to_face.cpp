// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zone_to_face.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(zoneToFace, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, zoneToFace, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, zoneToFace, istream);

}

mousse::topoSetSource::addToUsageTable mousse::zoneToFace::usage_
{
  zoneToFace::typeName,
  "\n    Usage: zoneToFace zone\n\n"
  "    Select all faces in the faceZone."
  " Note:accepts wildcards for zone.\n\n"
};


// Private Member Functions 
void mousse::zoneToFace::combine(topoSet& set, const bool add) const
{
  bool hasMatched = false;
  FOR_ALL(mesh_.faceZones(), i) {
    const faceZone& zone = mesh_.faceZones()[i];
    if (zoneName_.match(zone.name())) {
      const labelList& faceLabels = mesh_.faceZones()[i];
      Info << "    Found matching zone " << zone.name()
        << " with " << faceLabels.size() << " faces." << endl;
      hasMatched = true;
      FOR_ALL(faceLabels, i) {
        // Only do active faces
        if (faceLabels[i] < mesh_.nFaces()) {
          addOrDelete(set, faceLabels[i], add);
        }
      }
    }
  }
  if (!hasMatched) {
    WARNING_IN("zoneToFace::combine(topoSet&, const bool)")
      << "Cannot find any faceZone named " << zoneName_ << endl
      << "Valid names are " << mesh_.faceZones().names() << endl;
  }
}


// Constructors 

// Construct from components
mousse::zoneToFace::zoneToFace
(
  const polyMesh& mesh,
  const word& zoneName
)
:
  topoSetSource{mesh},
  zoneName_{zoneName}
{}


// Construct from dictionary
mousse::zoneToFace::zoneToFace
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  zoneName_{dict.lookup("name")}
{}


// Construct from Istream
mousse::zoneToFace::zoneToFace
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource{mesh},
  zoneName_{checkIs(is)}
{}


// Destructor 
mousse::zoneToFace::~zoneToFace()
{}


// Member Functions 
void mousse::zoneToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD)) {
    Info << "    Adding all faces of faceZone " << zoneName_ << " ..."
      << endl;
    combine(set, true);
  } else if (action == topoSetSource::DELETE) {
    Info << "    Removing all faces of faceZone " << zoneName_ << " ..."
      << endl;
    combine(set, false);
  }
}

