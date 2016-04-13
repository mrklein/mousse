#ifndef MESH_TOOLS_ALGORITHMS_PATCH_EDGE_FACE_WAVE_PATCH_EDGE_FACE_INFO_HPP_
#define MESH_TOOLS_ALGORITHMS_PATCH_EDGE_FACE_WAVE_PATCH_EDGE_FACE_INFO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchEdgeFaceInfo

#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "ptraits.hpp"
#include "primitive_patch.hpp"
#include "poly_mesh.hpp"
#include "transform.hpp"


namespace mousse {

// Forward declaration of classes
class polyPatch;
class polyMesh;


class patchEdgeFaceInfo
{
  // Private data
    //- Position of nearest wall center
    point origin_;
    //- Normal distance (squared) from point to origin
    scalar distSqr_;
  // Private Member Functions
    //- Evaluate distance to point. Update distSqr, origin from whomever
    //  is nearer pt. Return true if w2 is closer to point,
    //  false otherwise.
    template<class TrackingData>
    inline bool update
    (
      const point&,
      const patchEdgeFaceInfo& w2,
      const scalar tol,
      TrackingData& td
    );
    //- Combine current with w2. Update distSqr, origin if w2 has smaller
    //  quantities and returns true.
    template<class TrackingData>
    inline bool update
    (
      const patchEdgeFaceInfo& w2,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline patchEdgeFaceInfo();
    //- Construct from origin, distance
    inline patchEdgeFaceInfo(const point&, const scalar);
    //- Construct as copy
    inline patchEdgeFaceInfo(const patchEdgeFaceInfo&);
  // Member Functions
    // Access
      inline const point& origin() const;
      inline scalar distSqr() const;
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
        const primitivePatch& patch,
        const tensor& rotTensor,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of face on edge
      template<class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const primitivePatch& patch,
        const label edgeI,
        const label faceI,
        const patchEdgeFaceInfo& faceInfo,
        const scalar tol,
        TrackingData& td
      );
      //- New information for edge (from e.g. coupled edge)
      template<class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const primitivePatch& patch,
        const patchEdgeFaceInfo& edgeInfo,
        const bool sameOrientation,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of edge on face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh& mesh,
        const primitivePatch& patch,
        const label faceI,
        const label edgeI,
        const patchEdgeFaceInfo& edgeInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const patchEdgeFaceInfo&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const patchEdgeFaceInfo&) const;
    inline bool operator!=(const patchEdgeFaceInfo&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const patchEdgeFaceInfo&);
    friend Istream& operator>>(Istream&, patchEdgeFaceInfo&);
};

//- Data associated with patchEdgeFaceInfo type are contiguous
template<>
inline bool contiguous<patchEdgeFaceInfo>() {return true;}

}  // namespace mousse


// Private Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::patchEdgeFaceInfo::update
(
  const point& pt,
  const patchEdgeFaceInfo& w2,
  const scalar tol,
  TrackingData& td
)
{
  scalar dist2 = magSqr(pt - w2.origin());
  if (!valid(td)) {
    // current not yet set so use any value
    distSqr_ = dist2;
    origin_ = w2.origin();
    return true;
  }
  scalar diff = distSqr_ - dist2;
  if (diff < 0) {
    // already nearer to pt
    return false;
  }
  if ((diff < SMALL) || ((distSqr_ > SMALL) && (diff/distSqr_ < tol))) {
    // don't propagate small changes
    return false;
  } else {
    // update with new values
    distSqr_ = dist2;
    origin_ = w2.origin();
    return true;
  }
}


// Update this with w2 (information on same edge)
template<class TrackingData>
inline bool mousse::patchEdgeFaceInfo::update
(
  const patchEdgeFaceInfo& w2,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td)) {
    // current not yet set so use any value
    distSqr_ = w2.distSqr();
    origin_ = w2.origin();
    return true;
  }
  scalar diff = distSqr_ - w2.distSqr();
  if (diff < 0) {
    // already nearer to pt
    return false;
  }
  if ((diff < SMALL) || ((distSqr_ > SMALL) && (diff/distSqr_ < tol))) {
    // don't propagate small changes
    return false;
  } else {
    // update with new values
    distSqr_ =  w2.distSqr();
    origin_ = w2.origin();
    return true;
  }
}


// Constructors 
// Null constructor
inline mousse::patchEdgeFaceInfo::patchEdgeFaceInfo()
:
  origin_{point::max},
  distSqr_{sqr(GREAT)}
{}


// Construct from origin, distance
inline mousse::patchEdgeFaceInfo::patchEdgeFaceInfo
(
  const point& origin,
  const scalar distSqr
)
:
  origin_{origin},
  distSqr_{distSqr}
{}


// Construct as copy
inline mousse::patchEdgeFaceInfo::patchEdgeFaceInfo(const patchEdgeFaceInfo& wpt)
:
  origin_{wpt.origin()},
  distSqr_{wpt.distSqr()}
{}


// Member Functions 
inline const mousse::point& mousse::patchEdgeFaceInfo::origin() const
{
  return origin_;
}


inline mousse::scalar mousse::patchEdgeFaceInfo::distSqr() const
{
  return distSqr_;
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceInfo::valid(TrackingData&) const
{
  return origin_ != point::max;
}


template<class TrackingData>
inline void mousse::patchEdgeFaceInfo::transform
(
  const polyMesh&,
  const primitivePatch&,
  const tensor& rotTensor,
  const scalar /*tol*/,
  TrackingData&
)
{
  origin_ = mousse::transform(rotTensor, origin_);
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceInfo::updateEdge
(
  const polyMesh&,
  const primitivePatch& patch,
  const label edgeI,
  const label /*faceI*/,
  const patchEdgeFaceInfo& faceInfo,
  const scalar tol,
  TrackingData& td
)
{
  const edge& e = patch.edges()[edgeI];
  point eMid = 0.5*(patch.points()[patch.meshPoints()[e[0]]]
                    + patch.points()[patch.meshPoints()[e[1]]]);
  return update(eMid, faceInfo, tol, td);
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceInfo::updateEdge
(
  const polyMesh&,
  const primitivePatch&,
  const patchEdgeFaceInfo& edgeInfo,
  const bool /*sameOrientation*/,
  const scalar tol,
  TrackingData& td
)
{
  return update(edgeInfo, tol, td);
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceInfo::updateFace
(
  const polyMesh&,
  const primitivePatch& patch,
  const label faceI,
  const label /*edgeI*/,
  const patchEdgeFaceInfo& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(patch.faceCentres()[faceI], edgeInfo, tol, td);
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceInfo::equal
(
  const patchEdgeFaceInfo& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}


// Member Operators 
inline bool mousse::patchEdgeFaceInfo::operator==
(
  const mousse::patchEdgeFaceInfo& rhs
) const
{
  return origin() == rhs.origin();
}


inline bool mousse::patchEdgeFaceInfo::operator!=
(
  const mousse::patchEdgeFaceInfo& rhs
) const
{
  return !(*this == rhs);
}

#endif
