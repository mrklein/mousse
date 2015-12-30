// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(labelToCell, 0);
addToRunTimeSelectionTable(topoSetSource, labelToCell, word);
addToRunTimeSelectionTable(topoSetSource, labelToCell, istream);
}
mousse::topoSetSource::addToUsageTable mousse::labelToCell::usage_
(
  labelToCell::typeName,
  "\n    Usage: labelToCell (i0 i1 .. in)\n\n"
  "    Select cells by cellLabel\n\n"
);
// Private Member Functions 
void mousse::labelToCell::combine(topoSet& set, const bool add) const
{
  forAll(labels_, labelI)
  {
    addOrDelete(set, labels_[labelI], add);
  }
}
// Constructors 
// Construct from components
mousse::labelToCell::labelToCell
(
  const polyMesh& mesh,
  const labelList& labels
)
:
  topoSetSource(mesh),
  labels_(labels)
{}
// Construct from dictionary
mousse::labelToCell::labelToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  labels_(dict.lookup("value"))
{}
// Construct from Istream
mousse::labelToCell::labelToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  labels_(checkIs(is))
{}
// Destructor 
mousse::labelToCell::~labelToCell()
{}
// Member Functions 
void mousse::labelToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells mentioned in dictionary" << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells mentioned in dictionary" << " ..." << endl;
    combine(set, false);
  }
}
