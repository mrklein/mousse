// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_zone_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(faceZoneToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, faceZoneToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, faceZoneToCell, istream);

template<>
const char* mousse::NamedEnum
<
  mousse::faceZoneToCell::faceAction,
  2
>::names[] =
{
  "master",
  "slave"
};

}

mousse::topoSetSource::addToUsageTable mousse::faceZoneToCell::usage_
{
  faceZoneToCell::typeName,
  "\n    Usage: faceZoneToCell zone master|slave\n\n"
  "    Select master or slave side of the faceZone."
  " Note:accepts wildcards for zone.\n\n"
};

const mousse::NamedEnum<mousse::faceZoneToCell::faceAction, 2>
  mousse::faceZoneToCell::faceActionNames_;


// Private Member Functions 
void mousse::faceZoneToCell::combine(topoSet& set, const bool add) const
{
  bool hasMatched = false;
  FOR_ALL(mesh_.faceZones(), i) {
    const faceZone& zone = mesh_.faceZones()[i];
    if (zoneName_.match(zone.name())) {
      const labelList& cellLabels =
      (
        option_ == MASTER
       ? zone.masterCells()
       : zone.slaveCells()
      );
      Info << "    Found matching zone " << zone.name()
        << " with " << cellLabels.size() << " cells on selected side."
        << endl;
      hasMatched = true;
      FOR_ALL(cellLabels, i) {
        // Only do active cells
        if (cellLabels[i] < mesh_.nCells()) {
          addOrDelete(set, cellLabels[i], add);
        }
      }
    }
  }
  if (!hasMatched) {
    WARNING_IN("faceZoneToCell::combine(topoSet&, const bool)")
      << "Cannot find any faceZone named " << zoneName_ << endl
      << "Valid names are " << mesh_.faceZones().names() << endl;
  }
}


// Constructors 

// Construct from components
mousse::faceZoneToCell::faceZoneToCell
(
  const polyMesh& mesh,
  const word& zoneName,
  const faceAction option
)
:
  topoSetSource{mesh},
  zoneName_{zoneName},
  option_{option}
{}


// Construct from dictionary
mousse::faceZoneToCell::faceZoneToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  zoneName_{dict.lookup("name")},
  option_{faceActionNames_.read(dict.lookup("option"))}
{}


// Construct from Istream
mousse::faceZoneToCell::faceZoneToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource{mesh},
  zoneName_{checkIs(is)},
  option_{faceActionNames_.read(checkIs(is))}
{}


// Destructor 
mousse::faceZoneToCell::~faceZoneToCell()
{}


// Member Functions 
void mousse::faceZoneToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD)) {
    Info << "    Adding all " << faceActionNames_[option_]
      << " cells of faceZone " << zoneName_ << " ..." << endl;
    combine(set, true);
  } else if (action == topoSetSource::DELETE) {
    Info << "    Removing all " << faceActionNames_[option_]
      << " cells of faceZone " << zoneName_ << " ..." << endl;
    combine(set, false);
  }
}

