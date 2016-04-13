#ifndef CORE_MESHES_BOUND_BOX_HPP_
#define CORE_MESHES_BOUND_BOX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::boundBox
// Description
//   A bounding box defined in terms of the points at its extremities.

#include "point_field.hpp"
#include "face_list.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class boundBox;

template<class T> class tmp;
Istream& operator>>(Istream&, boundBox&);
Ostream& operator<<(Ostream&, const boundBox&);


class boundBox
{
  // Private data
    //- Minimum and maximum describing the bounding box
    point min_, max_;
  // Private Member Functions
    //- Calculate the bounding box from the given points.
    //  Does parallel communication (doReduce = true)
    void calculate(const UList<point>&, const bool doReduce = true);
public:
  // Static data members
    //- The great value used for greatBox and invertedBox
    static const scalar great;
    //- A very large boundBox: min/max == -/+ VGREAT
    static const boundBox greatBox;
    //- A very large inverted boundBox: min/max == +/- VGREAT
    static const boundBox invertedBox;
  // Constructors
    //- Construct null, setting points to zero
    inline boundBox();
    //- Construct from components
    inline boundBox(const point& min, const point& max);
    //- Construct as the bounding box of the given points
    //  Does parallel communication (doReduce = true)
    boundBox(const UList<point>&, const bool doReduce = true);
    //- Construct as the bounding box of the given temporary pointField.
    //  Does parallel communication (doReduce = true)
    boundBox(const tmp<pointField>&, const bool doReduce = true);
    //- Construct bounding box as subset of the pointField.
    //  The indices could be from cell/face etc.
    //  Does parallel communication (doReduce = true)
    boundBox
    (
      const UList<point>&,
      const labelUList& indices,
      const bool doReduce = true
    );
    //- Construct bounding box as subset of the pointField.
    //  The indices could be from edge/triFace etc.
    //  Does parallel communication (doReduce = true)
    template<unsigned Size>
    boundBox
    (
      const UList<point>&,
      const FixedList<label, Size>& indices,
      const bool doReduce = true
    );
    //- Construct from Istream
    inline boundBox(Istream&);
  // Member functions
    // Access
      //- Minimum describing the bounding box
      inline const point& min() const;
      //- Maximum describing the bounding box
      inline const point& max() const;
      //- Minimum describing the bounding box, non-const access
      inline point& min();
      //- Maximum describing the bounding box, non-const access
      inline point& max();
      //- The midpoint of the bounding box
      inline point midpoint() const;
      //- The bounding box span (from minimum to maximum)
      inline vector span() const;
      //- The magnitude of the bounding box span
      inline scalar mag() const;
      //- The volume of the bound box
      inline scalar volume() const;
      //- Smallest length/height/width dimension
      inline scalar minDim() const;
      //- Largest length/height/width dimension
      inline scalar maxDim() const;
      //- Average length/height/width dimension
      inline scalar avgDim() const;
      //- Return corner points in an order corresponding to a 'hex' cell
      tmp<pointField> points() const;
      //- Return faces with correct point order
      static faceList faces();
    // Manipulate
      //- Inflate box by factor*mag(span) in all dimensions
      void inflate(const scalar s);
    // Query
      //- Overlaps/touches boundingBox?
      inline bool overlaps(const boundBox&) const;
      //- Overlaps boundingSphere (centre + sqr(radius))?
      inline bool overlaps(const point&, const scalar radiusSqr) const;
      //- Contains point? (inside or on edge)
      inline bool contains(const point&) const;
      //- Fully contains other boundingBox?
      inline bool contains(const boundBox&) const;
      //- Contains point? (inside only)
      inline bool containsInside(const point&) const;
      //- Contains all of the points? (inside or on edge)
      bool contains(const UList<point>&) const;
      //- Contains all of the points? (inside or on edge)
      bool contains
      (
        const UList<point>&,
        const labelUList& indices
      ) const;
      //- Contains all of the points? (inside or on edge)
      template<unsigned Size>
      bool contains
      (
        const UList<point>&,
        const FixedList<label, Size>& indices
      ) const;
      //- Contains any of the points? (inside or on edge)
      bool containsAny(const UList<point>&) const;
      //- Contains any of the points? (inside or on edge)
      bool containsAny
      (
        const UList<point>&,
        const labelUList& indices
      ) const;
      //- Contains any of the points? (inside or on edge)
      template<unsigned Size>
      bool containsAny
      (
        const UList<point>&,
        const FixedList<label, Size>& indices
      ) const;
      //- Return the nearest point on the boundBox to the supplied point.
      //  If point is inside the boundBox then the point is returned
      //  unchanged.
      point nearest(const point&) const;
  // Friend Operators
    inline friend bool operator==(const boundBox&, const boundBox&);
    inline friend bool operator!=(const boundBox&, const boundBox&);
  // IOstream operator
    friend Istream& operator>>(Istream&, boundBox&);
    friend Ostream& operator<<(Ostream&, const boundBox&);
};

//- Data associated with boundBox type are contiguous
template<>
inline bool contiguous<boundBox>() {return contiguous<point>();}

}  // namespace mousse


// Constructors 
inline mousse::boundBox::boundBox()
:
  min_{point::zero},
  max_{point::zero}
{}


inline mousse::boundBox::boundBox(const point& min, const point& max)
:
  min_{min},
  max_{max}
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
  return 0.5*(max_ + min_);
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
  return bb.max_.x() >= min_.x() && bb.min_.x() <= max_.x()
    && bb.max_.y() >= min_.y() && bb.min_.y() <= max_.y()
    && bb.max_.z() >= min_.z() && bb.min_.z() <= max_.z();
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
  for (direction dir = 0; dir < vector::nComponents; dir++) {
    scalar d0 = min_[dir] - centre[dir];
    scalar d1 = max_[dir] - centre[dir];
    if ((d0 > 0) != (d1 > 0)) {
      // centre inside both extrema. This component does not add any
      // distance.
    } else if (mousse::mag(d0) < mousse::mag(d1)) {
      distSqr += d0*d0;
    } else {
      distSqr += d1*d1;
    }
    if (distSqr > radiusSqr) {
      return false;
    }
  }
  return true;
}


inline bool mousse::boundBox::contains(const point& pt) const
{
  return pt.x() >= min_.x() && pt.x() <= max_.x()
    && pt.y() >= min_.y() && pt.y() <= max_.y()
    && pt.z() >= min_.z() && pt.z() <= max_.z();
}


// this.bb fully contains bb
inline bool mousse::boundBox::contains(const boundBox& bb) const
{
  return contains(bb.min()) && contains(bb.max());
}


inline bool mousse::boundBox::containsInside(const point& pt) const
{
  return pt.x() > min_.x() && pt.x() < max_.x()
    && pt.y() > min_.y() && pt.y() < max_.y()
    && pt.z() > min_.z() && pt.z() < max_.z();
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

#include "bound_box.ipp"

#endif
