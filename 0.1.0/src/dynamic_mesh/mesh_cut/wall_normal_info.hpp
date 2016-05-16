#ifndef DYNAMIC_MESH_MESH_CUT_WALL_LAYER_CELLS_WALL_NORMAL_INFO_WALL_NORMAL_INFO_HPP_
#define DYNAMIC_MESH_MESH_CUT_WALL_LAYER_CELLS_WALL_NORMAL_INFO_WALL_NORMAL_INFO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallNormalInfo
// Description
//   Holds information regarding nearest wall point.
//   Used in wall refinement.

#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "poly_mesh.hpp"


namespace mousse {

// Forward declaration of classes
class polyPatch;
class polyMesh;


class wallNormalInfo
{
  // Private data
    //- Normal at nearest wall point
    vector normal_;
  // Private Member Functions
    //- Evaluate distance to point. Update normal_
    template<class TrackingData>
    inline bool update(const wallNormalInfo& w2, TrackingData& td);
public:
  // Constructors
    //- Construct null
    inline wallNormalInfo();
    //- Construct from normal
    inline wallNormalInfo(const vector& normal);
    //- Construct as copy
    inline wallNormalInfo(const wallNormalInfo&);
  // Member Functions
    // Access
      inline const vector& normal() const;
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
        const wallNormalInfo&,
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
        const wallNormalInfo& neighbourInfo,
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
        const wallNormalInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const wallNormalInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const wallNormalInfo&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const wallNormalInfo&) const;
    inline bool operator!=(const wallNormalInfo&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const wallNormalInfo&);
    friend Istream& operator>>(Istream&, wallNormalInfo&);
};

//- Data associated with wallNormalInfo type are contiguous
template<> inline bool contiguous<wallNormalInfo>() { return true;}

}  // namespace mousse


// Private Member Functions 

// Update this with w2 if not yet set.
template<class TrackingData>
inline bool mousse::wallNormalInfo::update
(
  const wallNormalInfo& w2,
  TrackingData& td
)
{
  if (!w2.valid(td)) {
    FATAL_ERROR_IN
    (
      "wallNormalInfo::update(const wallNormalInfo&)"
    )
    << "Problem: w2 is not valid" << abort(FatalError);
    return false;
  } else if (valid(td)) {
    // our already set. Stop any transfer
    return false;
  } else {
    normal_ = w2.normal();
    return true;
  }
}


// Constructors 
// Null constructor
inline mousse::wallNormalInfo::wallNormalInfo()
:
  normal_{point::max}
{}


// Construct from normal
inline mousse::wallNormalInfo::wallNormalInfo(const vector& normal)
:
  normal_{normal}
{}


// Construct as copy
inline mousse::wallNormalInfo::wallNormalInfo(const wallNormalInfo& wpt)
:
  normal_{wpt.normal()}
{}


// Member Functions 
inline const mousse::vector& mousse::wallNormalInfo::normal() const
{
  return normal_;
}


template<class TrackingData>
inline bool mousse::wallNormalInfo::valid(TrackingData&) const
{
  return normal_ != point::max;
}


// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::wallNormalInfo::sameGeometry
(
  const polyMesh&,
  const wallNormalInfo& /*w2*/,
  const scalar /*tol*/,
  TrackingData&
) const
{
  return true;
}


// No geometric data.
template<class TrackingData>
inline void mousse::wallNormalInfo::leaveDomain
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
inline void mousse::wallNormalInfo::transform
(
  const polyMesh&,
  const tensor& /*rotTensor*/,
  TrackingData&
)
{}


// No geometric data.
template<class TrackingData>
inline void mousse::wallNormalInfo::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}


// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallNormalInfo::updateCell
(
  const polyMesh&,
  const label /*thisCellI*/,
  const label /*neighbourFaceI*/,
  const wallNormalInfo& neighbourWallInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  return update(neighbourWallInfo, td);
}


// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallNormalInfo::updateFace
(
  const polyMesh&,
  const label /*thisFaceI*/,
  const label /*neighbourCellI*/,
  const wallNormalInfo& neighbourWallInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  return update(neighbourWallInfo, td);
}


// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallNormalInfo::updateFace
(
  const polyMesh&,
  const label /*thisFaceI*/,
  const wallNormalInfo& neighbourWallInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  return update(neighbourWallInfo, td);
}


template<class TrackingData>
inline bool mousse::wallNormalInfo::equal
(
  const wallNormalInfo& rhs,
  TrackingData& /*td*/
) const
{
  return operator==(rhs);
}


// Member Operators 
inline bool mousse::wallNormalInfo::operator==(const mousse::wallNormalInfo& rhs)
const
{
  return normal() == rhs.normal();
}


inline bool mousse::wallNormalInfo::operator!=(const mousse::wallNormalInfo& rhs)
const
{
  return !(*this == rhs);
}

#endif
