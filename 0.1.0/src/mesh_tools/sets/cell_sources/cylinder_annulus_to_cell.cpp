// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cylinder_annulus_to_cell.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cylinderAnnulusToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, cylinderAnnulusToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, cylinderAnnulusToCell, istream);

}

mousse::topoSetSource::addToUsageTable mousse::cylinderAnnulusToCell::usage_
{
  cylinderAnnulusToCell::typeName,
  "\n    Usage: cylinderAnnulusToCell (p1X p1Y p1Z) (p2X p2Y p2Z)"
  " outerRadius innerRadius\n\n"
  "    Select all cells with cell centre within bounding cylinder annulus\n\n"
};


// Private Member Functions 
void mousse::cylinderAnnulusToCell::combine(topoSet& set, const bool add) const
{
  const vector axis = p2_ - p1_;
  const scalar orad2 = sqr(outerRadius_);
  const scalar irad2 = sqr(innerRadius_);
  const scalar magAxis2 = magSqr(axis);
  const pointField& ctrs = mesh_.cellCentres();
  FOR_ALL(ctrs, cellI) {
    vector d = ctrs[cellI] - p1_;
    scalar magD = d & axis;
    if ((magD > 0) && (magD < magAxis2)) {
      scalar d2 = (d & d) - sqr(magD)/magAxis2;
      if ((d2 < orad2) && (d2 > irad2)) {
        addOrDelete(set, cellI, add);
      }
    }
  }
}


// Constructors 
mousse::cylinderAnnulusToCell::cylinderAnnulusToCell
(
  const polyMesh& mesh,
  const vector& p1,
  const vector& p2,
  const scalar outerRadius,
  const scalar innerRadius
)
:
  topoSetSource{mesh},
  p1_{p1},
  p2_{p2},
  outerRadius_{outerRadius},
  innerRadius_{innerRadius}
{}


mousse::cylinderAnnulusToCell::cylinderAnnulusToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  p1_{dict.lookup("p1")},
  p2_{dict.lookup("p2")},
  outerRadius_{readScalar(dict.lookup("outerRadius"))},
  innerRadius_{readScalar(dict.lookup("innerRadius"))}
{}


// Construct from Istream
mousse::cylinderAnnulusToCell::cylinderAnnulusToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource{mesh},
  p1_{checkIs(is)},
  p2_{checkIs(is)},
  outerRadius_{readScalar(checkIs(is))},
  innerRadius_{readScalar(checkIs(is))}
{}


// Destructor 
mousse::cylinderAnnulusToCell::~cylinderAnnulusToCell()
{}


// Member Functions 
void mousse::cylinderAnnulusToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD)) {
    Info << "    Adding cells with centre within cylinder annulus,"
      << " with p1 = "
      << p1_ << ", p2 = " << p2_ << " and outer radius = " << outerRadius_
      << " and inner radius = " << innerRadius_
      << endl;
    combine(set, true);
  } else if (action == topoSetSource::DELETE) {
    Info << "    Removing cells with centre within cylinder, with p1 = "
      << p1_ << ", p2 = " << p2_ << " and outer radius = " << outerRadius_
      << " and inner radius " << innerRadius_
      << endl;
    combine(set, false);
  }
}

