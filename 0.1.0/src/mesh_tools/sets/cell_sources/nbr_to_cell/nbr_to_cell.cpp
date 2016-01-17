// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nbr_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(nbrToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, nbrToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, nbrToCell, istream);
}

mousse::topoSetSource::addToUsageTable mousse::nbrToCell::usage_
(
  nbrToCell::typeName,
  "\n    Usage: nbrToCell <nNeighbours>\n\n"
  "    Select all cells with <= nNeighbours neighbouring cells\n\n"
);

// Private Member Functions 
void mousse::nbrToCell::combine(topoSet& set, const bool add) const
{
  const cellList& cells = mesh().cells();
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  boolList isCoupled(mesh_.nFaces()-mesh_.nInternalFaces(), false);
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    if (pp.coupled())
    {
      label faceI = pp.start();
      FOR_ALL(pp, i)
      {
        isCoupled[faceI-mesh_.nInternalFaces()] = true;
        faceI++;
      }
    }
  }
  FOR_ALL(cells, cellI)
  {
    const cell& cFaces = cells[cellI];
    label nNbrCells = 0;
    FOR_ALL(cFaces, i)
    {
      label faceI = cFaces[i];
      if (mesh_.isInternalFace(faceI))
      {
        nNbrCells++;
      }
      else if (isCoupled[faceI-mesh_.nInternalFaces()])
      {
        nNbrCells++;
      }
    }
    if (nNbrCells <= minNbrs_)
    {
      addOrDelete(set, cellI, add);
    }
  }
}
// Constructors 
// Construct from components
mousse::nbrToCell::nbrToCell
(
  const polyMesh& mesh,
  const label minNbrs
)
:
  topoSetSource(mesh),
  minNbrs_(minNbrs)
{}
// Construct from dictionary
mousse::nbrToCell::nbrToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  minNbrs_(readLabel(dict.lookup("neighbours")))
{}
// Construct from Istream
mousse::nbrToCell::nbrToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  minNbrs_(readLabel(checkIs(is)))
{}
// Destructor 
mousse::nbrToCell::~nbrToCell()
{}
// Member Functions 
void mousse::nbrToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells with only " << minNbrs_ << " or less"
        " neighbouring cells" << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells with only " << minNbrs_ << " or less"
        " neighbouring cells" << " ..." << endl;
    combine(set, false);
  }
}
