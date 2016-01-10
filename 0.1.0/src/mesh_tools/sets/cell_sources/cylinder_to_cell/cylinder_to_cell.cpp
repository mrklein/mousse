// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cylinder_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(cylinderToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, cylinderToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, cylinderToCell, istream);
}

mousse::topoSetSource::addToUsageTable mousse::cylinderToCell::usage_
(
  cylinderToCell::typeName,
  "\n    Usage: cylinderToCell (p1X p1Y p1Z) (p2X p2Y p2Z) radius\n\n"
  "    Select all cells with cell centre within bounding cylinder\n\n"
);

// Private Member Functions 
void mousse::cylinderToCell::combine(topoSet& set, const bool add) const
{
  const vector axis = p2_ - p1_;
  const scalar rad2 = sqr(radius_);
  const scalar magAxis2 = magSqr(axis);
  const pointField& ctrs = mesh_.cellCentres();
  FOR_ALL(ctrs, cellI)
  {
    vector d = ctrs[cellI] - p1_;
    scalar magD = d & axis;
    if ((magD > 0) && (magD < magAxis2))
    {
      scalar d2 = (d & d) - sqr(magD)/magAxis2;
      if (d2 < rad2)
      {
        addOrDelete(set, cellI, add);
      }
    }
  }
}
// Constructors 
mousse::cylinderToCell::cylinderToCell
(
  const polyMesh& mesh,
  const vector& p1,
  const vector& p2,
  const scalar radius
)
:
  topoSetSource(mesh),
  p1_(p1),
  p2_(p2),
  radius_(radius)
{}
mousse::cylinderToCell::cylinderToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  p1_(dict.lookup("p1")),
  p2_(dict.lookup("p2")),
  radius_(readScalar(dict.lookup("radius")))
{}
// Construct from Istream
mousse::cylinderToCell::cylinderToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  p1_(checkIs(is)),
  p2_(checkIs(is)),
  radius_(readScalar(checkIs(is)))
{}
// Destructor 
mousse::cylinderToCell::~cylinderToCell()
{}
// Member Functions 
void mousse::cylinderToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells with centre within cylinder, with p1 = "
      << p1_ << ", p2 = " << p2_ << " and radius = " << radius_ << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells with centre within cylinder, with p1 = "
      << p1_ << ", p2 = " << p2_ << " and radius = " << radius_ << endl;
    combine(set, false);
  }
}
