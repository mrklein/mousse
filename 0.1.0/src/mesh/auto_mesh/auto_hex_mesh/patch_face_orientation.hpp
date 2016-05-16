#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_MESH_REFINEMENT_PATCH_FACE_ORIENTATION_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_MESH_REFINEMENT_PATCH_FACE_ORIENTATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchFaceOrientation
// Description
//   Transport of orientation for use in PatchEdgeFaceWave.

#include "tensor.hpp"
#include "indirect_primitive_patch.hpp"
#include "poly_mesh.hpp"
#include "transform.hpp"
#include "oriented_surface.hpp"


namespace mousse {

// Forward declaration of classes
class polyMesh;


class patchFaceOrientation
{
  // Private data
    //- Flip status
    label flipStatus_;
public:
  // Constructors
    //- Construct null
    inline patchFaceOrientation();
    //- Construct from components
    inline patchFaceOrientation(const label);
  // Member Functions
    //- Orientation
    inline label flipStatus() const;
    //- Reverse orientation
    inline void flip();
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
        const patchFaceOrientation& faceInfo,
        const scalar tol,
        TrackingData& td
      );
      //- New information for edge (from e.g. coupled edge)
      template<class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const indirectPrimitivePatch& patch,
        const patchFaceOrientation& edgeInfo,
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
        const patchFaceOrientation& edgeInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const patchFaceOrientation&, TrackingData&) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const patchFaceOrientation&) const;
    inline bool operator!=(const patchFaceOrientation&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const patchFaceOrientation&);
    friend Istream& operator>>(Istream&, patchFaceOrientation&);
};

//- Data associated with patchFaceOrientation type are contiguous
template<>
inline bool contiguous<patchFaceOrientation>() { return true; }

}  // namespace mousse


// Private Member Functions 
inline mousse::patchFaceOrientation::patchFaceOrientation()
:
  flipStatus_{orientedSurface::UNVISITED}
{}


inline mousse::patchFaceOrientation::patchFaceOrientation
(
  const label flipStatus
)
:
  flipStatus_{flipStatus}
{}
// Member Functions 
inline mousse::label mousse::patchFaceOrientation::flipStatus() const
{
  return flipStatus_;
}


inline void mousse::patchFaceOrientation::flip()
{
  if (flipStatus_ == orientedSurface::NOFLIP) {
    flipStatus_ = orientedSurface::FLIP;
  } else if (flipStatus_ == orientedSurface::FLIP) {
    flipStatus_ = orientedSurface::NOFLIP;
  }
}


template<class TrackingData>
inline bool mousse::patchFaceOrientation::valid(TrackingData&) const
{
  return flipStatus_ != orientedSurface::UNVISITED;
}


template<class TrackingData>
inline void mousse::patchFaceOrientation::transform
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const tensor& /*rotTensor*/,
  const scalar /*tol*/,
  TrackingData& /*td*/
)
{}


template<class TrackingData>
inline bool mousse::patchFaceOrientation::updateEdge
(
  const polyMesh&,
  const indirectPrimitivePatch& patch,
  const label edgeI,
  const label faceI,
  const patchFaceOrientation& faceInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (valid(td)) {
    return false;
  }
  const face& f = patch.localFaces()[faceI];
  const edge& e = patch.edges()[edgeI];
  patchFaceOrientation consistentInfo{faceInfo};
  // Check how edge relates to face
  if (f.edgeDirection(e) < 0) {
    // Create flipped version of faceInfo
    consistentInfo.flip();
  }
  operator=(consistentInfo);
  return true;
}


template<class TrackingData>
inline bool mousse::patchFaceOrientation::updateEdge
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const patchFaceOrientation& edgeInfo,
  const bool sameOrientation,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (valid(td)) {
    return false;
  }
  // Create (flipped/unflipped) version of edgeInfo
  patchFaceOrientation consistentInfo{edgeInfo};
  if (!sameOrientation) {
    consistentInfo.flip();
  }
  operator=(consistentInfo);
  return true;
}


template<class TrackingData>
inline bool mousse::patchFaceOrientation::updateFace
(
  const polyMesh&,
  const indirectPrimitivePatch& patch,
  const label faceI,
  const label edgeI,
  const patchFaceOrientation& edgeInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (valid(td)) {
    return false;
  }
  // Transfer flip to face
  const face& f = patch.localFaces()[faceI];
  const edge& e = patch.edges()[edgeI];
  // Create (flipped/unflipped) version of edgeInfo
  patchFaceOrientation consistentInfo{edgeInfo};
  if (f.edgeDirection(e) > 0) {
    consistentInfo.flip();
  }
  operator=(consistentInfo);
  return true;
}


template<class TrackingData>
inline bool mousse::patchFaceOrientation::equal
(
  const patchFaceOrientation& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}


// Member Operators 
inline bool mousse::patchFaceOrientation::operator==
(
  const mousse::patchFaceOrientation& rhs
) const
{
  return flipStatus() == rhs.flipStatus();
}


inline bool mousse::patchFaceOrientation::operator!=
(
  const mousse::patchFaceOrientation& rhs
) const
{
  return !(*this == rhs);
}

#endif

