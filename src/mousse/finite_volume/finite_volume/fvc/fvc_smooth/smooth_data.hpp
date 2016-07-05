#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SMOOTH_SMOOTH_DATA_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SMOOTH_SMOOTH_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::smoothData
// Description
//   Helper class used by the fvc::smooth and fvc::spread functions.
// SourceFiles
//   smooth_data.hpp
namespace mousse
{
class smoothData
{
public:
  //- Class used to pass additional data in
  class trackData
  {
  public:
    //- Cut off distance
    scalar maxRatio;
  };
private:
  scalar value_;
  // Private Member Functions
    //- Update - gets information from neighbouring face/cell and
    //  uses this to update itself (if necessary) and return true
    template<class TrackingData>
    inline bool update
    (
      const smoothData& svf,
      const scalar scale,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline smoothData();
    //- Construct from inverse field value
    inline smoothData(const scalar value);
  // Member Functions
    // Access
      //- Return value
      scalar value() const
      {
        return value_;
      }
    // Needed by FaceCellWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data
      //  Used for cyclics checking
      template<class TrackingData>
      inline bool sameGeometry
      (
        const polyMesh&,
        const smoothData&,
        const scalar,
        TrackingData& td
      ) const;
      //- Convert any absolute coordinates into relative to
      //  (patch)face centre
      template<class TrackingData>
      inline void leaveDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Reverse of leaveDomain
      template<class TrackingData>
      inline void enterDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Apply rotation matrix to any coordinates
      template<class TrackingData>
      inline void transform
      (
        const polyMesh&,
        const tensor&,
        TrackingData& td
      );
      //- Influence of neighbouring face
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh&,
        const label thisCellI,
        const label neighbourFaceI,
        const smoothData& svf,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of neighbouring cell
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const label neighbourCellI,
        const smoothData& svf,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const smoothData& svf,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const smoothData&, TrackingData& td) const;
    // Member Operators
      inline void operator=(const scalar value);
      // Needed for List IO
      inline bool operator==(const smoothData&) const;
      inline bool operator!=(const smoothData&) const;
    // IOstream Operators
      friend Ostream& operator<<
      (
        Ostream& os,
        const smoothData& svf
      )
      {
        return os  << svf.value_;
      }
      friend Istream& operator>>(Istream& is, smoothData& svf)
      {
        return is  >> svf.value_;
      }
};
//- Data associated with smoothData type are contiguous
template<>
inline bool contiguous<smoothData>()
{
  return true;
}
}  // namespace mousse

// Private Member Functions 
template<class TrackingData>
inline bool mousse::smoothData::update
(
  const smoothData& svf,
  const scalar scale,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td) || (value_ < VSMALL))
  {
    // My value not set - take over neighbour
    value_ = svf.value()/scale;
    // Something changed - let caller know
    return true;
  }
  else if (svf.value() > (1 + tol)*scale*value_)
  {
    // Neighbour is too big for me - Up my value
    value_ = svf.value()/scale;
    // Something changed - let caller know
    return true;
  }
  else
  {
    // Neighbour is not too big for me or change is too small
    // Nothing changed
    return false;
  }
}
// Constructors 
inline mousse::smoothData::smoothData()
:
  value_{-GREAT}
{}
inline mousse::smoothData::smoothData(const scalar value)
:
  value_{value}
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::smoothData::valid(TrackingData&) const
{
  return value_ > -SMALL;
}
template<class TrackingData>
inline bool mousse::smoothData::sameGeometry
(
  const polyMesh&,
  const smoothData&,
  const scalar,
  TrackingData&
) const
{
  return true;
}
template<class TrackingData>
inline void mousse::smoothData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point&,
  TrackingData&
)
{}
template<class TrackingData>
inline void mousse::smoothData::transform
(
  const polyMesh&,
  const tensor&,
  TrackingData&
)
{}
template<class TrackingData>
inline void mousse::smoothData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point&,
  TrackingData&
)
{}
template<class TrackingData>
inline bool mousse::smoothData::updateCell
(
  const polyMesh&,
  const label,
  const label,
  const smoothData& svf,
  const scalar tol,
  TrackingData& td
)
{
  // Take over info from face if more than deltaRatio larger
  return update(svf, td.maxRatio, tol, td);
}
template<class TrackingData>
inline bool mousse::smoothData::updateFace
(
  const polyMesh&,
  const label,
  const label,
  const smoothData& svf,
  const scalar tol,
  TrackingData& td
)
{
  // Take over information from cell without any scaling (scale = 1.0)
  return update(svf, 1.0, tol, td);
}
// Update this (face) with coupled face information.
template<class TrackingData>
inline bool mousse::smoothData::updateFace
(
  const polyMesh&,
  const label,
  const smoothData& svf,
  const scalar tol,
  TrackingData& td
)
{
  // Take over information from coupled face without any scaling (scale = 1.0)
  return update(svf, 1.0, tol, td);
}
template<class TrackingData>
inline bool mousse::smoothData::equal
(
  const smoothData& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline void mousse::smoothData::operator=
(
  const scalar value
)
{
  value_ = value;
}
inline bool mousse::smoothData::operator==
(
  const smoothData& rhs
) const
{
  return value_ == rhs.value();
}
inline bool mousse::smoothData::operator!=
(
  const smoothData& rhs
) const
{
  return !(*this == rhs);
}
#endif
