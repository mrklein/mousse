// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "box_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(boxToCell, 0);
addToRunTimeSelectionTable(topoSetSource, boxToCell, word);
addToRunTimeSelectionTable(topoSetSource, boxToCell, istream);
}
mousse::topoSetSource::addToUsageTable mousse::boxToCell::usage_
(
  boxToCell::typeName,
  "\n    Usage: boxToCell (minx miny minz) (maxx maxy maxz)\n\n"
  "    Select all cells with cellCentre within bounding box\n\n"
);
// Private Member Functions 
void mousse::boxToCell::combine(topoSet& set, const bool add) const
{
  const pointField& ctrs = mesh_.cellCentres();
  forAll(ctrs, cellI)
  {
    forAll(bbs_, i)
    {
      if (bbs_[i].contains(ctrs[cellI]))
      {
        addOrDelete(set, cellI, add);
        break;
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::boxToCell::boxToCell
(
  const polyMesh& mesh,
  const treeBoundBoxList& bbs
)
:
  topoSetSource(mesh),
  bbs_(bbs)
{}
// Construct from dictionary
mousse::boxToCell::boxToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  bbs_
  (
    dict.found("box")
   ? treeBoundBoxList(1, treeBoundBox(dict.lookup("box")))
   : dict.lookup("boxes")
  )
{}
// Construct from Istream
mousse::boxToCell::boxToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  bbs_(1, treeBoundBox(checkIs(is)))
{}
// Destructor 
mousse::boxToCell::~boxToCell()
{}
// Member Functions 
void mousse::boxToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells with center within boxes " << bbs_ << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells with center within boxes " << bbs_ << endl;
    combine(set, false);
  }
}
