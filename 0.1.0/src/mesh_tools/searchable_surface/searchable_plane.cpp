// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_plane.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "sortable_list.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(searchablePlane, 0);
addToRunTimeSelectionTable(searchableSurface, searchablePlane, dict);
}
// Private Member Functions 
mousse::pointIndexHit mousse::searchablePlane::findLine
(
  const point& start,
  const point& end
) const
{
  pointIndexHit info(true, vector::zero, 0);
  linePointRef l(start, end);
  scalar t = lineIntersect(l);
  if (t < 0 || t > 1)
  {
    info.setMiss();
    info.setIndex(-1);
  }
  else
  {
    info.setPoint(start+t*l.vec());
  }
  return info;
}
mousse::boundBox mousse::searchablePlane::calcBounds() const
{
  point max(VGREAT, VGREAT, VGREAT);
  for (direction dir = 0; dir < vector::nComponents; dir++)
  {
    if (mag(normal()[dir]) - 1 < SMALL)
    {
      max[dir] = 0;
      break;
    }
  }
  point min = -max;
  return boundBox(min, max);
}
// Constructors 
mousse::searchablePlane::searchablePlane
(
  const IOobject& io,
  const point& basePoint,
  const vector& normal
)
:
  searchableSurface(io),
  plane(basePoint, normal)
{
  bounds() = calcBounds();
}
mousse::searchablePlane::searchablePlane
(
  const IOobject& io,
  const dictionary& dict
)
:
  searchableSurface(io),
  plane(dict)
{
  bounds() = calcBounds();
}
// Destructor 
mousse::searchablePlane::~searchablePlane()
{}
// Member Functions 
const mousse::wordList& mousse::searchablePlane::regions() const
{
  if (regions_.empty())
  {
    regions_.setSize(1);
    regions_[0] = "region0";
  }
  return regions_;
}
void mousse::searchablePlane::boundingSpheres
(
  pointField& centres,
  scalarField& radiusSqr
) const
{
  centres.setSize(1);
  centres[0] = refPoint();
  radiusSqr.setSize(1);
  radiusSqr[0] = mousse::sqr(GREAT);
}
void mousse::searchablePlane::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info
) const
{
  info.setSize(samples.size());
  forAll(samples, i)
  {
    info[i].setPoint(nearestPoint(samples[i]));
    if (magSqr(samples[i]-info[i].rawPoint()) > nearestDistSqr[i])
    {
      info[i].setIndex(-1);
      info[i].setMiss();
    }
    else
    {
      info[i].setIndex(0);
      info[i].setHit();
    }
  }
}
void mousse::searchablePlane::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  forAll(start, i)
  {
    info[i] = findLine(start[i], end[i]);
  }
}
void mousse::searchablePlane::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  findLine(start, end, info);
}
void mousse::searchablePlane::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit> >& info
) const
{
  List<pointIndexHit> nearestInfo;
  findLine(start, end, nearestInfo);
  info.setSize(start.size());
  forAll(info, pointI)
  {
    if (nearestInfo[pointI].hit())
    {
      info[pointI].setSize(1);
      info[pointI][0] = nearestInfo[pointI];
    }
    else
    {
      info[pointI].clear();
    }
  }
}
void mousse::searchablePlane::getRegion
(
  const List<pointIndexHit>& info,
  labelList& region
) const
{
  region.setSize(info.size());
  region = 0;
}
void mousse::searchablePlane::getNormal
(
  const List<pointIndexHit>& info,
  vectorField& n
) const
{
  n.setSize(info.size());
  n = normal();
}
void mousse::searchablePlane::getVolumeType
(
  const pointField& points,
  List<volumeType>& volType
) const
{
  FatalErrorIn
  (
    "searchableCollection::getVolumeType(const pointField&"
    ", List<volumeType>&) const"
  )   << "Volume type not supported for plane."
    << exit(FatalError);
}
