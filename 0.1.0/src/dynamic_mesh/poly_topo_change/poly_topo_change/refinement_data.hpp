#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REFINEMENT_DATA_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REFINEMENT_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refinementData
// Description
//   Transfers refinement levels such that slow transition between levels is
//   maintained. Used in FaceCellWave.
// SourceFiles
//   refinement_data.cpp
#include "point.hpp"
#include "tensor.hpp"
namespace mousse
{
class polyPatch;
class polyMesh;
class refinementData
{
  // Private data
    //- Count which triggers refinement
    label refinementCount_;
    //- Refinement level
    label count_;
public:
  // Constructors
    //- Construct null
    inline refinementData();
    //- Construct from count
    inline refinementData(const label refinementCount, const label count);
  // Member Functions
    // Access
      inline label refinementCount() const
      {
        return refinementCount_;
      }
      inline label& refinementCount()
      {
        return refinementCount_;
      }
      inline label count() const
      {
        return count_;
      }
      inline label& count()
      {
        return count_;
      }
      inline bool isRefined() const
      {
        return count_ >= refinementCount_;
      }
    // Needed by FaceCellWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const polyMesh&,
        const refinementData&,
        const scalar,
        TrackingData& td
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
      //- Influence of neighbouring face.
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh&,
        const label thisCellI,
        const label neighbourFaceI,
        const refinementData& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of neighbouring cell.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const label neighbourCellI,
        const refinementData& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const refinementData& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const refinementData&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const refinementData&) const;
    inline bool operator!=(const refinementData&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const refinementData&);
    friend Istream& operator>>(Istream&, refinementData&);
};
//- Data associated with refinementData type are contiguous
template<>
inline bool contiguous<refinementData>()
{
  return true;
}
}  // namespace mousse

// Constructors 
// Null constructor
inline mousse::refinementData::refinementData()
:
  refinementCount_{-1},
  count_{-1}
{}
// Construct from components
inline mousse::refinementData::refinementData
(
  const label refinementCount,
  const label count
)
:
  refinementCount_{refinementCount},
  count_{count}
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::refinementData::valid(TrackingData&) const
{
  return count_ != -1;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::refinementData::sameGeometry
(
  const polyMesh&,
  const refinementData&,
  const scalar,
  TrackingData&
) const
{
  return true;
}
// No geometric data.
template<class TrackingData>
inline void mousse::refinementData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::refinementData::transform
(
  const polyMesh&,
  const tensor& /*rotTensor*/,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::refinementData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}
// Update cell with neighbouring face information
template<class TrackingData>
inline bool mousse::refinementData::updateCell
(
  const polyMesh&,
  const label /*thisCellI*/,
  const label /*neighbourFaceI*/,
  const refinementData& neighbourInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (!valid(td))
  {
    FATAL_ERROR_IN("refinementData::updateCell") << "problem"
      << abort(FatalError);
    return false;
  }
  // Check if more than 2:1 ratio. This is when I am not refined but neighbour
  // is and neighbour already had higher cell level.
  if (neighbourInfo.isRefined() && !isRefined()
      &&  neighbourInfo.refinementCount() > refinementCount())
  {
    count_ = refinementCount();
    return true;
  }
  // Count from neighbour face by the time it reaches the current cell.
  label transportedFaceCount;
  if (neighbourInfo.isRefined())
  {
    // refined so passes through two cells.
    transportedFaceCount = max(0, neighbourInfo.count()-2);
  }
  else
  {
    // unrefined.
    transportedFaceCount = max(0, neighbourInfo.count()-1);
  }
  if (count_ >= transportedFaceCount)
  {
    return false;
  }
  else
  {
    count_ = transportedFaceCount;
    return true;
  }
}
// Update face with neighbouring cell information
template<class TrackingData>
inline bool mousse::refinementData::updateFace
(
  const polyMesh&,
  const label /*thisFaceI*/,
  const label /*neighbourCellI*/,
  const refinementData& neighbourInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  // From cell to its faces.
  if (!valid(td))
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
  if (count_ >= neighbourInfo.count())
  {
    return false;
  }
  else
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
}
// Update face with coupled face information
template<class TrackingData>
inline bool mousse::refinementData::updateFace
(
  const polyMesh&,
  const label /*thisFaceI*/,
  const refinementData& neighbourInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  // From face to face (e.g. coupled faces)
  if (!valid(td))
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
  if (count_ >= neighbourInfo.count())
  {
    return false;
  }
  else
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
}
template<class TrackingData>
inline bool mousse::refinementData::equal
(
  const refinementData& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::refinementData::operator==(const mousse::refinementData& rhs)
const
{
  return count() == rhs.count() && refinementCount() == rhs.refinementCount();
}
inline bool mousse::refinementData::operator!=(const mousse::refinementData& rhs)
const
{
  return !(*this == rhs);
}
#endif
