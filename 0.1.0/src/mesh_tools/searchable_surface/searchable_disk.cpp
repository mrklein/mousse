// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_disk.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(searchableDisk, 0);
addToRunTimeSelectionTable(searchableSurface, searchableDisk, dict);
}
// Private Member Functions 
mousse::pointIndexHit mousse::searchableDisk::findNearest
(
  const point& sample,
  const scalar nearestDistSqr
) const
{
  pointIndexHit info(false, sample, -1);
  vector v(sample - origin_);
  // Decompose sample-origin into normal and parallel component
  scalar parallel = (v & normal_);
  // Remove the parallel component and normalise
  v -= parallel*normal_;
  scalar magV = mag(v);
  if (magV < ROOTVSMALL)
  {
    v = vector::zero;
  }
  else
  {
    v /= magV;
  }
  // Clip to radius.
  info.setPoint(origin_ + min(magV, radius_)*v);
  if (magSqr(sample - info.rawPoint()) < nearestDistSqr)
  {
    info.setHit();
    info.setIndex(0);
  }
  return info;
}
void mousse::searchableDisk::findLine
(
  const point& start,
  const point& end,
  pointIndexHit& info
) const
{
  info = pointIndexHit(false, vector::zero, -1);
  vector v(start - origin_);
  // Decompose sample-origin into normal and parallel component
  scalar parallel = (v & normal_);
  if (sign(parallel) == sign((end - origin_) & normal_))
  {
    return;
  }
  // Remove the parallel component and normalise
  v -= parallel*normal_;
  scalar magV = mag(v);
  if (magV < ROOTVSMALL)
  {
    v = vector::zero;
  }
  else
  {
    v /= magV;
  }
  // Set (hit or miss) to intersection of ray and plane of disk
  info.setPoint(origin_ + magV*v);
  if (magV <= radius_)
  {
    info.setHit();
    info.setIndex(0);
  }
}
// Constructors 
mousse::searchableDisk::searchableDisk
(
  const IOobject& io,
  const point& origin,
  const point& normal,
  const scalar radius
)
:
  searchableSurface(io),
  origin_(origin),
  normal_(normal/mag(normal)),
  radius_(radius)
{
  // Rough approximation of bounding box
  //vector span(radius_, radius_, radius_);
  // See searchableCylinder
  vector span
  (
    sqrt(sqr(normal_.y()) + sqr(normal_.z())),
    sqrt(sqr(normal_.x()) + sqr(normal_.z())),
    sqrt(sqr(normal_.x()) + sqr(normal_.y()))
  );
  span *= radius_;
  bounds().min() = origin_ - span;
  bounds().max() = origin_ + span;
}
mousse::searchableDisk::searchableDisk
(
  const IOobject& io,
  const dictionary& dict
)
:
  searchableSurface(io),
  origin_(dict.lookup("origin")),
  normal_(dict.lookup("normal")),
  radius_(readScalar(dict.lookup("radius")))
{
  normal_ /= mag(normal_);
  // Rough approximation of bounding box
  //vector span(radius_, radius_, radius_);
  // See searchableCylinder
  vector span
  (
    sqrt(sqr(normal_.y()) + sqr(normal_.z())),
    sqrt(sqr(normal_.x()) + sqr(normal_.z())),
    sqrt(sqr(normal_.x()) + sqr(normal_.y()))
  );
  span *= radius_;
  bounds().min() = origin_ - span;
  bounds().max() = origin_ + span;
}
// Destructor 
mousse::searchableDisk::~searchableDisk()
{}
// Member Functions 
const mousse::wordList& mousse::searchableDisk::regions() const
{
  if (regions_.empty())
  {
    regions_.setSize(1);
    regions_[0] = "region0";
  }
  return regions_;
}
void mousse::searchableDisk::boundingSpheres
(
  pointField& centres,
  scalarField& radiusSqr
) const
{
  centres.setSize(1);
  centres[0] = origin_;
  radiusSqr.setSize(1);
  radiusSqr[0] = sqr(radius_);
  // Add a bit to make sure all points are tested inside
  radiusSqr += mousse::sqr(SMALL);
}
void mousse::searchableDisk::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info
) const
{
  info.setSize(samples.size());
  forAll(samples, i)
  {
    info[i] = findNearest(samples[i], nearestDistSqr[i]);
  }
}
void mousse::searchableDisk::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  forAll(start, i)
  {
    findLine(start[i], end[i], info[i]);
  }
}
void mousse::searchableDisk::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  findLine(start, end, info);
}
void mousse::searchableDisk::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit> >& info
) const
{
  info.setSize(start.size());
  forAll(start, i)
  {
    pointIndexHit inter;
    findLine(start[i], end[i], inter);
    if (inter.hit())
    {
      info[i].setSize(1);
      info[i][0] = inter;
    }
    else
    {
      info[i].clear();
    }
  }
}
void mousse::searchableDisk::getRegion
(
  const List<pointIndexHit>& info,
  labelList& region
) const
{
  region.setSize(info.size());
  region = 0;
}
void mousse::searchableDisk::getNormal
(
  const List<pointIndexHit>& info,
  vectorField& normal
) const
{
  normal.setSize(info.size());
  normal = normal_;
}
void mousse::searchableDisk::getVolumeType
(
  const pointField& points,
  List<volumeType>& volType
) const
{
  FatalErrorIn
  (
    "searchableDisk::getVolumeType(const pointField&"
    ", List<volumeType>&) const"
  )   << "Volume type not supported for disk."
    << exit(FatalError);
}
