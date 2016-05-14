// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_zone_selection.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"


// Static Data Members
namespace mousse {
namespace faceSelections {

DEFINE_TYPE_NAME_AND_DEBUG(faceZoneSelection, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(faceSelection, faceZoneSelection, dictionary);

}
}


// Constructors 
mousse::faceSelections::faceZoneSelection::faceZoneSelection
(
  const word& name,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  faceSelection{name, mesh, dict},
  zoneName_{dict_.lookup("zoneName")}
{}


// Destructor 
mousse::faceSelections::faceZoneSelection::~faceZoneSelection()
{}


// Member Functions 
void mousse::faceSelections::faceZoneSelection::select
(
  const label zoneID,
  labelList& faceToZoneID,
  boolList& faceToFlip
) const
{
  label readID = mesh_.faceZones().findZoneID(zoneName_);
  if (readID == -1) {
    FATAL_ERROR_IN
    (
      "faceSelections::faceZoneSelection::select(labelList&) const"
    )
    << "Cannot find faceZone " << zoneName_ << nl << "Valid zones are "
    << mesh_.faceZones().names()
    << exit(FatalError);
  }
  const faceZone& fZone = mesh_.faceZones()[readID];
  FOR_ALL(fZone, i) {
    label faceI = fZone[i];
    if (faceToZoneID[faceI] == -1) {
      faceToZoneID[faceI] = zoneID;
      faceToFlip[faceI] = fZone.flipMap()[i];
    } else if (faceToZoneID[faceI] != zoneID) {
      FATAL_ERROR_IN
      (
        "faceSelections::faceZoneSelection::select(labelList&) const"
      )
      << "Face " << faceI << " already in faceZone "
      << faceToZoneID[faceI]
      << exit(FatalError);
    }
  }
  faceSelection::select(zoneID, faceToZoneID, faceToFlip);
}

