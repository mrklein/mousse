// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "box_to_point.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(boxToPoint, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, boxToPoint, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, boxToPoint, istream);
}

mousse::topoSetSource::addToUsageTable mousse::boxToPoint::usage_
(
  boxToPoint::typeName,
  "\n    Usage: boxToPoint ((minx miny minz) (maxx maxy maxz))\n\n"
  "    Select all points with coordinate within bounding box\n\n"
);

// Private Member Functions 
void mousse::boxToPoint::combine(topoSet& set, const bool add) const
{
  const pointField& pts = mesh_.points();
  FOR_ALL(pts, pointI)
  {
    FOR_ALL(bbs_, i)
    {
      if (bbs_[i].contains(pts[pointI]))
      {
        addOrDelete(set, pointI, add);
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::boxToPoint::boxToPoint
(
  const polyMesh& mesh,
  const treeBoundBoxList& bbs
)
:
  topoSetSource(mesh),
  bbs_(bbs)
{}
// Construct from dictionary
mousse::boxToPoint::boxToPoint
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
mousse::boxToPoint::boxToPoint
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  bbs_(1, treeBoundBox(checkIs(is)))
{}
// Destructor 
mousse::boxToPoint::~boxToPoint()
{}
// Member Functions 
void mousse::boxToPoint::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding points that are within boxes " << bbs_ << " ..."
      << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing points that are within boxes " << bbs_ << " ..."
      << endl;
    combine(set, false);
  }
}
