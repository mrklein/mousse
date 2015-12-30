// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchEdgeFaceInfo
// Description
// SourceFiles
//   patch_edge_face_info_i.hpp
//   patch_edge_face_info.cpp
#ifndef patch_edge_face_info_hpp_
#define patch_edge_face_info_hpp_
#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "ptraits.hpp"
#include "primitive_patch.hpp"
namespace mousse
{
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
inline bool contiguous<patchEdgeFaceInfo>()
{
  return true;
}
}  // namespace mousse
#include "patch_edge_face_info_i.hpp"
#endif
