// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refinementDistanceData
// Description
//   Transfers refinement levels such that slow transition between levels is
//   maintained. Used in FaceCellWave.
// SourceFiles
//   refinement_distance_data.cpp
#ifndef refinement_distance_data_hpp_
#define refinement_distance_data_hpp_
#include "point.hpp"
#include "tensor.hpp"
#include "transform.hpp"
#include "poly_mesh.hpp"
namespace mousse
{
class polyPatch;
class polyMesh;
class refinementDistanceData
{
  // Private data
    //- Unrefined (level0) buffer size (nBufferLayers*level0Size)
    scalar level0Size_;
    //- Nearest point with highest level
    point origin_;
    label originLevel_;
  // Private Member Functions
    //- Updates with neighbouring data. Returns true if something changed.
    template<class TrackingData>
    inline bool update
    (
      const point&,
      const refinementDistanceData& neighbourInfo,
      const scalar tol,
      TrackingData&
    );
public:
  // Constructors
    //- Construct null
    inline refinementDistanceData();
    //- Construct from count
    inline refinementDistanceData
    (
      const scalar level0Size,
      const point& origin,
      const label level
    );
  // Member Functions
    // Access
      inline scalar level0Size() const
      {
        return level0Size_;
      }
      inline scalar& level0Size()
      {
        return level0Size_;
      }
      inline const point& origin() const
      {
        return origin_;
      }
      inline point& origin()
      {
        return origin_;
      }
      inline label originLevel() const
      {
        return originLevel_;
      }
      inline label& originLevel()
      {
        return originLevel_;
      }
    // Other
      //- Calculates the wanted level at a given point. Walks out from
      //  the origin.
      inline label wantedLevel(const point& pt) const;
    // Needed by FaceCellWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData&) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const polyMesh&,
        const refinementDistanceData&,
        const scalar,
        TrackingData&
      ) const;
      //- Convert any absolute coordinates into relative to (patch)face
      //  centre
      template<class TrackingData>
      inline void leaveDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData&
      );
      //- Reverse of leaveDomain
      template<class TrackingData>
      inline void enterDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData&
      );
      //- Apply rotation matrix to any coordinates
      template<class TrackingData>
      inline void transform
      (
        const polyMesh&,
        const tensor&,
        TrackingData&
      );
      //- Influence of neighbouring face.
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh&,
        const label thisCellI,
        const label neighbourFaceI,
        const refinementDistanceData& neighbourInfo,
        const scalar tol,
        TrackingData&
      );
      //- Influence of neighbouring cell.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const label neighbourCellI,
        const refinementDistanceData& neighbourInfo,
        const scalar tol,
        TrackingData&
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const refinementDistanceData& neighbourInfo,
        const scalar tol,
        TrackingData&
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal
      (
        const refinementDistanceData&,
        TrackingData&
      ) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const refinementDistanceData&) const;
    inline bool operator!=(const refinementDistanceData&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const refinementDistanceData&);
    friend Istream& operator>>(Istream&, refinementDistanceData&);
};
//- Data associated with refinementDistanceData type are contiguous
template<>
inline bool contiguous<refinementDistanceData>()
{
  return true;
}
}  // namespace mousse

// Private Member Functions 
// Returns the wanted level
inline mousse::label mousse::refinementDistanceData::wantedLevel(const point& pt)
const
{
  const scalar distSqr = magSqr(pt-origin_);
  // Get the size at the origin level
  scalar levelSize = level0Size_/(1<<originLevel_);
  scalar r = 0;
  for (label level = originLevel_; level >= 0; --level)
  {
    // Current range
    r += levelSize;
    // Check if our distance is within influence sphere
    if (sqr(r) > distSqr)
    {
      return level;
    }
    // Lower level will have double the size
    levelSize *= 2;
  }
  return 0;
}
template<class TrackingData>
inline bool mousse::refinementDistanceData::update
(
  const point& pos,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td))
  {
    if (!neighbourInfo.valid(td))
    {
      FATAL_ERROR_IN("refinementDistanceData::update(..)")
        << "problem" << abort(FatalError);
    }
    operator=(neighbourInfo);
    return true;
  }
  // Determine wanted level at current position.
  label cellLevel = wantedLevel(pos);
  // Determine wanted level coming through the neighbour
  label nbrLevel = neighbourInfo.wantedLevel(pos);
  if (nbrLevel > cellLevel)
  {
    operator=(neighbourInfo);
    return true;
  }
  else if (nbrLevel == cellLevel)
  {
    scalar myDistSqr = magSqr(pos-origin_);
    scalar nbrDistSqr = magSqr(pos - neighbourInfo.origin());
    scalar diff = myDistSqr - nbrDistSqr;
    if (diff < 0)
    {
      // already nearest
      return false;
    }
    if ((diff < SMALL) || ((myDistSqr > SMALL) && (diff/myDistSqr < tol)))
    {
      // don't propagate small changes
      return false;
    }
    else
    {
      // update with new values
      operator=(neighbourInfo);
      return true;
    }
  }
  else
  {
    return false;
  }
}
// Constructors 
// Null constructor
inline mousse::refinementDistanceData::refinementDistanceData()
:
  level0Size_{-1}
{}
// Construct from components
inline mousse::refinementDistanceData::refinementDistanceData
(
  const scalar level0Size,
  const point& origin,
  const label originLevel
)
:
  level0Size_{level0Size},
  origin_{origin},
  originLevel_{originLevel}
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::refinementDistanceData::valid(TrackingData&) const
{
  return level0Size_ != -1;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::refinementDistanceData::sameGeometry
(
  const polyMesh&,
  const refinementDistanceData&,
  const scalar,
  TrackingData&
) const
{
  return true;
}
template<class TrackingData>
inline void mousse::refinementDistanceData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& faceCentre,
  TrackingData&
)
{
  origin_ -= faceCentre;
}
template<class TrackingData>
inline void mousse::refinementDistanceData::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData&
)
{
  origin_ = mousse::transform(rotTensor, origin_);
}
// Update absolute geometric quantities.
template<class TrackingData>
inline void mousse::refinementDistanceData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& faceCentre,
  TrackingData&
)
{
  // back to absolute form
  origin_ += faceCentre;
}
// Update cell with neighbouring face information
template<class TrackingData>
inline bool mousse::refinementDistanceData::updateCell
(
  const polyMesh& mesh,
  const label thisCellI,
  const label /*neighbourFaceI*/,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  const point& pos = mesh.cellCentres()[thisCellI];
  return update(pos, neighbourInfo, tol, td);
}
// Update face with neighbouring cell information
template<class TrackingData>
inline bool mousse::refinementDistanceData::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label /*neighbourCellI*/,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  const point& pos = mesh.faceCentres()[thisFaceI];
  return update(pos, neighbourInfo, tol, td);
}
// Update face with coupled face information
template<class TrackingData>
inline bool mousse::refinementDistanceData::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  const point& pos = mesh.faceCentres()[thisFaceI];
  return update(pos, neighbourInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::refinementDistanceData::equal
(
  const refinementDistanceData& rhs,
  TrackingData& td
) const
{
  if (!valid(td))
  {
    if (!rhs.valid(td))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return operator==(rhs);
  }
}
// Member Operators 
inline bool mousse::refinementDistanceData::operator==
(
  const mousse::refinementDistanceData& rhs
)
const
{
  return (level0Size_ == rhs.level0Size_ && origin_ == rhs.origin_
          && originLevel_ == rhs.originLevel_);
}
inline bool mousse::refinementDistanceData::operator!=
(
  const mousse::refinementDistanceData& rhs
)
const
{
  return !(*this == rhs);
}
#endif
