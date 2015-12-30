// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_sphere.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(searchableSphere, 0);
addToRunTimeSelectionTable(searchableSurface, searchableSphere, dict);
}
// Private Member Functions 
mousse::pointIndexHit mousse::searchableSphere::findNearest
(
  const point& sample,
  const scalar nearestDistSqr
) const
{
  pointIndexHit info(false, sample, -1);
  const vector n(sample - centre_);
  scalar magN = mag(n);
  if (nearestDistSqr >= sqr(magN - radius_))
  {
    if (magN < ROOTVSMALL)
    {
      info.rawPoint() = centre_ + vector(1,0,0)*radius_;
    }
    else
    {
      info.rawPoint() = centre_ + n/magN*radius_;
    }
    info.setHit();
    info.setIndex(0);
  }
  return info;
}
// From Graphics Gems - intersection of sphere with ray
void mousse::searchableSphere::findLineAll
(
  const point& start,
  const point& end,
  pointIndexHit& near,
  pointIndexHit& far
) const
{
  near.setMiss();
  far.setMiss();
  vector dir(end-start);
  scalar magSqrDir = magSqr(dir);
  if (magSqrDir > ROOTVSMALL)
  {
    const vector toCentre(centre_-start);
    scalar magSqrToCentre = magSqr(toCentre);
    dir /= mousse::sqrt(magSqrDir);
    scalar v = (toCentre & dir);
    scalar disc = sqr(radius_) - (magSqrToCentre - sqr(v));
    if (disc >= 0)
    {
      scalar d = mousse::sqrt(disc);
      scalar nearParam = v-d;
      if (nearParam >= 0 && sqr(nearParam) <= magSqrDir)
      {
        near.setHit();
        near.setPoint(start + nearParam*dir);
        near.setIndex(0);
      }
      scalar farParam = v+d;
      if (farParam >= 0 && sqr(farParam) <= magSqrDir)
      {
        far.setHit();
        far.setPoint(start + farParam*dir);
        far.setIndex(0);
      }
    }
  }
}
// Constructors 
mousse::searchableSphere::searchableSphere
(
  const IOobject& io,
  const point& centre,
  const scalar radius
)
:
  searchableSurface(io),
  centre_(centre),
  radius_(radius)
{
  bounds() = boundBox
  (
    centre_ - radius_*vector::one,
    centre_ + radius_*vector::one
  );
}
mousse::searchableSphere::searchableSphere
(
  const IOobject& io,
  const dictionary& dict
)
:
  searchableSurface(io),
  centre_(dict.lookup("centre")),
  radius_(readScalar(dict.lookup("radius")))
{
  bounds() = boundBox
  (
    centre_ - radius_*vector::one,
    centre_ + radius_*vector::one
  );
}
// Destructor 
mousse::searchableSphere::~searchableSphere()
{}
// Member Functions 
bool mousse::searchableSphere::overlaps(const boundBox& bb) const
{
  return bb.overlaps(centre_, sqr(radius_));
}
const mousse::wordList& mousse::searchableSphere::regions() const
{
  if (regions_.empty())
  {
    regions_.setSize(1);
    regions_[0] = "region0";
  }
  return regions_;
}
void mousse::searchableSphere::boundingSpheres
(
  pointField& centres,
  scalarField& radiusSqr
) const
{
  centres.setSize(1);
  centres[0] = centre_;
  radiusSqr.setSize(1);
  radiusSqr[0] = mousse::sqr(radius_);
  // Add a bit to make sure all points are tested inside
  radiusSqr += mousse::sqr(SMALL);
}
void mousse::searchableSphere::findNearest
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
void mousse::searchableSphere::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  pointIndexHit b;
  forAll(start, i)
  {
    // Pick nearest intersection. If none intersected take second one.
    findLineAll(start[i], end[i], info[i], b);
    if (!info[i].hit() && b.hit())
    {
      info[i] = b;
    }
  }
}
void mousse::searchableSphere::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  pointIndexHit b;
  forAll(start, i)
  {
    // Discard far intersection
    findLineAll(start[i], end[i], info[i], b);
    if (!info[i].hit() && b.hit())
    {
      info[i] = b;
    }
  }
}
void mousse::searchableSphere::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit> >& info
) const
{
  info.setSize(start.size());
  forAll(start, i)
  {
    pointIndexHit near, far;
    findLineAll(start[i], end[i], near, far);
    if (near.hit())
    {
      if (far.hit())
      {
        info[i].setSize(2);
        info[i][0] = near;
        info[i][1] = far;
      }
      else
      {
        info[i].setSize(1);
        info[i][0] = near;
      }
    }
    else
    {
      if (far.hit())
      {
        info[i].setSize(1);
        info[i][0] = far;
      }
      else
      {
        info[i].clear();
      }
    }
  }
}
void mousse::searchableSphere::getRegion
(
  const List<pointIndexHit>& info,
  labelList& region
) const
{
  region.setSize(info.size());
  region = 0;
}
void mousse::searchableSphere::getNormal
(
  const List<pointIndexHit>& info,
  vectorField& normal
) const
{
  normal.setSize(info.size());
  normal = vector::zero;
  forAll(info, i)
  {
    if (info[i].hit())
    {
      normal[i] = info[i].hitPoint() - centre_;
      normal[i] /= mag(normal[i])+VSMALL;
    }
    else
    {
      // Set to what?
    }
  }
}
void mousse::searchableSphere::getVolumeType
(
  const pointField& points,
  List<volumeType>& volType
) const
{
  volType.setSize(points.size());
  volType = volumeType::INSIDE;
  forAll(points, pointI)
  {
    const point& pt = points[pointI];
    if (magSqr(pt - centre_) <= sqr(radius_))
    {
      volType[pointI] = volumeType::INSIDE;
    }
    else
    {
      volType[pointI] = volumeType::OUTSIDE;
    }
  }
}
