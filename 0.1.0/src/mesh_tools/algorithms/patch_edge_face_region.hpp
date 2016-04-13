#ifndef MESH_TOOLS_ALGORITHMS_PATCH_EDGE_FACE_WAVE_PATCH_EDGE_FACE_REGION_HPP_
#define MESH_TOOLS_ALGORITHMS_PATCH_EDGE_FACE_WAVE_PATCH_EDGE_FACE_REGION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchEdgeFaceRegion
// Description
//   Transport of region for use in PatchEdgeFaceWave.
//   Set element to -2 to denote blocked.

#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "indirect_primitive_patch.hpp"
#include "poly_mesh.hpp"
#include "transform.hpp"


namespace mousse {

// Forward declaration of classes
class polyPatch;
class polyMesh;


class patchEdgeFaceRegion
{
  // Private data
    //- region
    label region_;
  // Private Member Functions
    //- Combine current with w2. Update region_ if w2 has smaller
    //  quantities and returns true.
    template<class TrackingData>
    inline bool update
    (
      const patchEdgeFaceRegion& w2,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline patchEdgeFaceRegion();
    //- Construct from origin, distance
    inline patchEdgeFaceRegion(const label);
  // Member Functions
    // Access
      inline label region() const;
    // Needed by meshWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Apply rotation matrix
      template<class TrackingData>
      inline void transform
      (
        const polyMesh& mesh,
        const indirectPrimitivePatch& patch,
        const tensor& rotTensor,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of face on edge
      template<class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const indirectPrimitivePatch& patch,
        const label edgeI,
        const label faceI,
        const patchEdgeFaceRegion& faceInfo,
        const scalar tol,
        TrackingData& td
      );
      //- New information for edge (from e.g. coupled edge)
      template<class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const indirectPrimitivePatch& patch,
        const patchEdgeFaceRegion& edgeInfo,
        const bool sameOrientation,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of edge on face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh& mesh,
        const indirectPrimitivePatch& patch,
        const label faceI,
        const label edgeI,
        const patchEdgeFaceRegion& edgeInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const patchEdgeFaceRegion&, TrackingData&) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const patchEdgeFaceRegion&) const;
    inline bool operator!=(const patchEdgeFaceRegion&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const patchEdgeFaceRegion&);
    friend Istream& operator>>(Istream&, patchEdgeFaceRegion&);
};

//- Data associated with patchEdgeFaceRegion type are contiguous
template<>
inline bool contiguous<patchEdgeFaceRegion>() {return true;}

}  // namespace mousse


// Private Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::update
(
  const patchEdgeFaceRegion& w2,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (!w2.valid(td)) {
    FATAL_ERROR_IN("patchEdgeFaceRegion::update(..)")
      << "problem." << abort(FatalError);
  }
  if (w2.region_ == -2 || region_ == -2) {
    // Blocked edge/face
    return false;
  }
  if (!valid(td)) {
    // current not yet set so use any value
    operator=(w2);
    return true;
  } else {
    if (w2.region_ < region_) {
      operator=(w2);
      return true;
    } else {
      return false;
    }
  }
}


// Constructors 

// Null constructor
inline mousse::patchEdgeFaceRegion::patchEdgeFaceRegion()
:
  region_{-1}
{}


// Construct from origin, distance
inline mousse::patchEdgeFaceRegion::patchEdgeFaceRegion
(
  const label region
)
:
  region_{region}
{}


// Member Functions 
inline mousse::label mousse::patchEdgeFaceRegion::region() const
{
  return region_;
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::valid(TrackingData&) const
{
  return region_ != -1;
}


template<class TrackingData>
inline void mousse::patchEdgeFaceRegion::transform
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const tensor& /*rotTensor*/,
  const scalar /*tol*/,
  TrackingData&
)
{}


template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::updateEdge
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const label /*edgeI*/,
  const label /*faceI*/,
  const patchEdgeFaceRegion& faceInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(faceInfo, tol, td);
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::updateEdge
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const patchEdgeFaceRegion& edgeInfo,
  const bool /*sameOrientation*/,
  const scalar tol,
  TrackingData& td
)
{
  return update(edgeInfo, tol, td);
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::updateFace
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const label /*faceI*/,
  const label /*edgeI*/,
  const patchEdgeFaceRegion& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(edgeInfo, tol, td);
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::equal
(
  const patchEdgeFaceRegion& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}


// Member Operators 
inline bool mousse::patchEdgeFaceRegion::operator==
(
  const mousse::patchEdgeFaceRegion& rhs
) const
{
  return region() == rhs.region();
}


inline bool mousse::patchEdgeFaceRegion::operator!=
(
  const mousse::patchEdgeFaceRegion& rhs
) const
{
  return !(*this == rhs);
}

#endif
