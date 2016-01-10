// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bound_box.hpp"
#include "point_field.hpp"
// Constructors 
inline mousse::boundBox::boundBox()
:
  min_(point::zero),
  max_(point::zero)
{}
inline mousse::boundBox::boundBox(const point& min, const point& max)
:
  min_(min),
  max_(max)
{}
inline mousse::boundBox::boundBox(Istream& is)
{
  operator>>(is, *this);
}
// Member Functions 
inline const mousse::point& mousse::boundBox::min() const
{
  return min_;
}
inline const mousse::point& mousse::boundBox::max() const
{
  return max_;
}
inline mousse::point& mousse::boundBox::min()
{
  return min_;
}
inline mousse::point& mousse::boundBox::max()
{
  return max_;
}
inline mousse::point mousse::boundBox::midpoint() const
{
  return 0.5 * (max_ + min_);
}
inline mousse::vector mousse::boundBox::span() const
{
  return (max_ - min_);
}
inline mousse::scalar mousse::boundBox::mag() const
{
  return ::mousse::mag(max_ - min_);
}
inline mousse::scalar mousse::boundBox::volume() const
{
  return cmptProduct(span());
}
inline mousse::scalar mousse::boundBox::minDim() const
{
  return cmptMin(span());
}
inline mousse::scalar mousse::boundBox::maxDim() const
{
  return cmptMax(span());
}
inline mousse::scalar mousse::boundBox::avgDim() const
{
  return cmptAv(span());
}
inline bool mousse::boundBox::overlaps(const boundBox& bb) const
{
  return
  (
    bb.max_.x() >= min_.x() && bb.min_.x() <= max_.x()
  && bb.max_.y() >= min_.y() && bb.min_.y() <= max_.y()
  && bb.max_.z() >= min_.z() && bb.min_.z() <= max_.z()
  );
}
inline bool mousse::boundBox::overlaps
(
  const point& centre,
  const scalar radiusSqr
) const
{
  // Find out where centre is in relation to bb.
  // Find nearest point on bb.
  scalar distSqr = 0;
  for (direction dir = 0; dir < vector::nComponents; dir++)
  {
    scalar d0 = min_[dir] - centre[dir];
    scalar d1 = max_[dir] - centre[dir];
    if ((d0 > 0) != (d1 > 0))
    {
      // centre inside both extrema. This component does not add any
      // distance.
    }
    else if (mousse::mag(d0) < mousse::mag(d1))
    {
      distSqr += d0*d0;
    }
    else
    {
      distSqr += d1*d1;
    }
    if (distSqr > radiusSqr)
    {
      return false;
    }
  }
  return true;
}
inline bool mousse::boundBox::contains(const point& pt) const
{
  return
  (
    pt.x() >= min_.x() && pt.x() <= max_.x()
  && pt.y() >= min_.y() && pt.y() <= max_.y()
  && pt.z() >= min_.z() && pt.z() <= max_.z()
  );
}
// this.bb fully contains bb
inline bool mousse::boundBox::contains(const boundBox& bb) const
{
  return contains(bb.min()) && contains(bb.max());
}
inline bool mousse::boundBox::containsInside(const point& pt) const
{
  return
  (
    pt.x() > min_.x() && pt.x() < max_.x()
  && pt.y() > min_.y() && pt.y() < max_.y()
  && pt.z() > min_.z() && pt.z() < max_.z()
  );
}
// Friend Operators 
inline bool mousse::operator==(const boundBox& a, const boundBox& b)
{
  return (a.min_ == b.min_) && (a.max_ == b.max_);
}
inline bool mousse::operator!=(const boundBox& a, const boundBox& b)
{
  return !(a == b);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
