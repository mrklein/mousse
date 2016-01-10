// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tree_bound_box.hpp"
#include "random.hpp"
// Constructors 
inline mousse::treeBoundBox::treeBoundBox()
:
  boundBox()
{}
inline mousse::treeBoundBox::treeBoundBox(const point& min, const point& max)
:
  boundBox(min, max)
{}
inline mousse::treeBoundBox::treeBoundBox(const boundBox& bb)
:
  boundBox(bb)
{}
inline mousse::treeBoundBox::treeBoundBox(Istream& is)
:
  boundBox(is)
{}
// Member Functions 
inline mousse::scalar mousse::treeBoundBox::typDim() const
{
  return avgDim();
}
inline mousse::point mousse::treeBoundBox::corner(const direction octant) const
{
  return point
  (
    (octant & RIGHTHALF) ? max().x() : min().x(),
    (octant & TOPHALF)   ? max().y() : min().y(),
    (octant & FRONTHALF) ? max().z() : min().z()
  );
}
// Returns octant in which point resides. Reverse of subBbox.
inline mousse::direction mousse::treeBoundBox::subOctant(const point& pt) const
{
  return subOctant(midpoint(), pt);
}
// Returns octant in which point resides. Reverse of subBbox.
// Precalculated midpoint
inline mousse::direction mousse::treeBoundBox::subOctant
(
  const point& mid,
  const point& pt
)
{
  direction octant = 0;
  if (pt.x() > mid.x())
  {
    octant |= treeBoundBox::RIGHTHALF;
  }
  if (pt.y() > mid.y())
  {
    octant |= treeBoundBox::TOPHALF;
  }
  if (pt.z() > mid.z())
  {
    octant |= treeBoundBox::FRONTHALF;
  }
  return octant;
}
// Returns octant in which point resides. Reverse of subBbox.
// Flags point exactly on edge.
inline mousse::direction mousse::treeBoundBox::subOctant
(
  const point& pt,
  bool& onEdge
) const
{
  return subOctant(midpoint(), pt, onEdge);
}
// Returns octant in which point resides. Reverse of subBbox.
// Precalculated midpoint
inline mousse::direction mousse::treeBoundBox::subOctant
(
  const point& mid,
  const point& pt,
  bool& onEdge
)
{
  direction octant = 0;
  onEdge = false;
  if (pt.x() > mid.x())
  {
    octant |= treeBoundBox::RIGHTHALF;
  }
  else if (pt.x() == mid.x())
  {
    onEdge = true;
  }
  if (pt.y() > mid.y())
  {
    octant |= treeBoundBox::TOPHALF;
  }
  else if (pt.y() == mid.y())
  {
    onEdge = true;
  }
  if (pt.z() > mid.z())
  {
    octant |= treeBoundBox::FRONTHALF;
  }
  else if (pt.z() == mid.z())
  {
    onEdge = true;
  }
  return octant;
}
// Returns octant in which intersection resides.
// Precalculated midpoint. If the point is on the dividing line between
// the octants the direction vector determines which octant to use
// (i.e. in which octant the point would be if it were moved along dir)
inline mousse::direction mousse::treeBoundBox::subOctant
(
  const point& mid,
  const vector& dir,
  const point& pt,
  bool& onEdge
)
{
  direction octant = 0;
  onEdge = false;
  if (pt.x() > mid.x())
  {
    octant |= treeBoundBox::RIGHTHALF;
  }
  else if (pt.x() == mid.x())
  {
    onEdge = true;
    if (dir.x() > 0)
    {
      octant |= treeBoundBox::RIGHTHALF;
    }
  }
  if (pt.y() > mid.y())
  {
    octant |= treeBoundBox::TOPHALF;
  }
  else if (pt.y() == mid.y())
  {
    onEdge = true;
    if (dir.y() > 0)
    {
      octant |= treeBoundBox::TOPHALF;
    }
  }
  if (pt.z() > mid.z())
  {
    octant |= treeBoundBox::FRONTHALF;
  }
  else if (pt.z() == mid.z())
  {
    onEdge = true;
    if (dir.z() > 0)
    {
      octant |= treeBoundBox::FRONTHALF;
    }
  }
  return octant;
}
// Returns reference to octantOrder which defines the
// order to do the search.
inline void mousse::treeBoundBox::searchOrder
(
  const point& pt,
  FixedList<direction,8>& octantOrder
) const
{
  vector dist = midpoint() - pt;
  direction octant = 0;
  if (dist.x() < 0)
  {
    octant |= treeBoundBox::RIGHTHALF;
    dist.x() *= -1;
  }
  if (dist.y() < 0)
  {
    octant |= treeBoundBox::TOPHALF;
    dist.y() *= -1;
  }
  if (dist.z() < 0)
  {
    octant |= treeBoundBox::FRONTHALF;
    dist.z() *= -1;
  }
  direction min = 0;
  direction mid = 0;
  direction max = 0;
  if (dist.x() < dist.y())
  {
    if (dist.y() < dist.z())
    {
      min = treeBoundBox::RIGHTHALF;
      mid = treeBoundBox::TOPHALF;
      max = treeBoundBox::FRONTHALF;
    }
    else if (dist.z() < dist.x())
    {
      min = treeBoundBox::FRONTHALF;
      mid = treeBoundBox::RIGHTHALF;
      max = treeBoundBox::TOPHALF;
    }
    else
    {
      min = treeBoundBox::RIGHTHALF;
      mid = treeBoundBox::FRONTHALF;
      max = treeBoundBox::TOPHALF;
    }
  }
  else
  {
    if (dist.z() < dist.y())
    {
      min = treeBoundBox::FRONTHALF;
      mid = treeBoundBox::TOPHALF;
      max = treeBoundBox::RIGHTHALF;
    }
    else if (dist.x() < dist.z())
    {
      min = treeBoundBox::TOPHALF;
      mid = treeBoundBox::RIGHTHALF;
      max = treeBoundBox::FRONTHALF;
    }
    else
    {
      min = treeBoundBox::TOPHALF;
      mid = treeBoundBox::FRONTHALF;
      max = treeBoundBox::RIGHTHALF;
    }
  }
  // Primary subOctant
  octantOrder[0] = octant;
  // subOctants joined to the primary by faces.
  octantOrder[1] = octant ^ min;
  octantOrder[2] = octant ^ mid;
  octantOrder[3] = octant ^ max;
  // subOctants joined to the primary by edges.
  octantOrder[4] = octantOrder[1] ^ mid;
  octantOrder[5] = octantOrder[1] ^ max;
  octantOrder[6] = octantOrder[2] ^ max;
  // subOctants joined to the primary by corner.
  octantOrder[7] = octantOrder[4] ^ max;
}
//- Return slightly wider bounding box
inline mousse::treeBoundBox mousse::treeBoundBox::extend
(
  Random& rndGen,
  const scalar s
) const
{
  treeBoundBox bb(*this);
  vector newSpan = bb.span();
  // Make 3D
  scalar minSpan = s * mousse::mag(newSpan);
  for (direction dir = 0; dir < vector::nComponents; dir++)
  {
    newSpan[dir] = mousse::max(newSpan[dir], minSpan);
  }
  bb.min() -= cmptMultiply(s * rndGen.vector01(), newSpan);
  bb.max() += cmptMultiply(s * rndGen.vector01(), newSpan);
  return bb;
}
