// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zone_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(zoneToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, zoneToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, zoneToCell, istream);

}

mousse::topoSetSource::addToUsageTable mousse::zoneToCell::usage_
{
  zoneToCell::typeName,
  "\n    Usage: zoneToCell zone\n\n"
  "    Select all cells in the cellZone."
  " Note:accepts wildcards for zone.\n\n"
};


// Private Member Functions 

void mousse::zoneToCell::combine(topoSet& set, const bool add) const
{
  bool hasMatched = false;
  FOR_ALL(mesh_.cellZones(), i) {
    const cellZone& zone = mesh_.cellZones()[i];
    if (zoneName_.match(zone.name())) {
      const labelList& cellLabels = mesh_.cellZones()[i];
      Info << "    Found matching zone " << zone.name()
        << " with " << cellLabels.size() << " cells." << endl;
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
    WARNING_IN("zoneToCell::combine(topoSet&, const bool)")
      << "Cannot find any cellZone named " << zoneName_ << endl
      << "Valid names are " << mesh_.cellZones().names() << endl;
  }
}


// Constructors 

// Construct from components
mousse::zoneToCell::zoneToCell
(
  const polyMesh& mesh,
  const word& zoneName
)
:
  topoSetSource{mesh},
  zoneName_{zoneName}
{}


// Construct from dictionary
mousse::zoneToCell::zoneToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  zoneName_{dict.lookup("name")}
{}


// Construct from Istream
mousse::zoneToCell::zoneToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource{mesh},
  zoneName_{checkIs(is)}
{}


// Destructor 
mousse::zoneToCell::~zoneToCell()
{}


// Member Functions 
void mousse::zoneToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD)) {
    Info << "    Adding all cells of cellZone " << zoneName_ << " ..."
      << endl;
    combine(set, true);
  } else if (action == topoSetSource::DELETE) {
    Info << "    Removing all cells of cellZone " << zoneName_ << " ..."
      << endl;
    combine(set, false);
  }
}

