// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sphere_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(sphereToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, sphereToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, sphereToCell, istream);
}

mousse::topoSetSource::addToUsageTable mousse::sphereToCell::usage_
(
  sphereToCell::typeName,
  "\n    Usage: sphereToCell (centreX centreY centreZ) radius\n\n"
  "    Select all cells with cellCentre within bounding sphere\n\n"
);

// Private Member Functions 
void mousse::sphereToCell::combine(topoSet& set, const bool add) const
{
  const pointField& ctrs = mesh_.cellCentres();
  const scalar radSquared = radius_*radius_;
  FOR_ALL(ctrs, cellI)
  {
    scalar offset = magSqr(centre_ - ctrs[cellI]);
    if (offset <= radSquared)
    {
      addOrDelete(set, cellI, add);
    }
  }
}
// Constructors 
mousse::sphereToCell::sphereToCell
(
  const polyMesh& mesh,
  const vector& centre,
  const scalar radius
)
:
  topoSetSource(mesh),
  centre_(centre),
  radius_(radius)
{}
mousse::sphereToCell::sphereToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  centre_(dict.lookup("centre")),
  radius_(readScalar(dict.lookup("radius")))
{}
// Construct from Istream
mousse::sphereToCell::sphereToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  centre_(checkIs(is)),
  radius_(readScalar(checkIs(is)))
{}
// Destructor 
mousse::sphereToCell::~sphereToCell()
{}
// Member Functions 
void mousse::sphereToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells with centre within sphere, with centre = "
      << centre_ << " and radius = " << radius_ << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells with centre within sphere, with centre = "
      << centre_ << " and radius = " << radius_ << endl;
    combine(set, false);
  }
}
