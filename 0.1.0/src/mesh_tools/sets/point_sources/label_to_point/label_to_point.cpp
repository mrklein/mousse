// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_to_point.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(labelToPoint, 0);
addToRunTimeSelectionTable(topoSetSource, labelToPoint, word);
addToRunTimeSelectionTable(topoSetSource, labelToPoint, istream);
}
mousse::topoSetSource::addToUsageTable mousse::labelToPoint::usage_
(
  labelToPoint::typeName,
  "\n    Usage: labelToPoint (i0 i1 .. in)\n\n"
  "    Select points by label\n\n"
);
// Private Member Functions 
void mousse::labelToPoint::combine(topoSet& set, const bool add) const
{
  forAll(labels_, labelI)
  {
    addOrDelete(set, labels_[labelI], add);
  }
}
// Constructors 
// Construct from components
mousse::labelToPoint::labelToPoint
(
  const polyMesh& mesh,
  const labelList& labels
)
:
  topoSetSource(mesh),
  labels_(labels)
{}
// Construct from dictionary
mousse::labelToPoint::labelToPoint
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  labels_(dict.lookup("value"))
{}
// Construct from Istream
mousse::labelToPoint::labelToPoint
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  labels_(checkIs(is))
{}
// Destructor 
mousse::labelToPoint::~labelToPoint()
{}
// Member Functions 
void mousse::labelToPoint::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding points mentioned in dictionary" << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing points mentioned in dictionary" << " ..."
      << endl;
    combine(set, false);
  }
}
