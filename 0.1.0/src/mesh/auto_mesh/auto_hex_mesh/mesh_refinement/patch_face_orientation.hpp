// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchFaceOrientation
// Description
//   Transport of orientation for use in PatchEdgeFaceWave.
// SourceFiles
//   patch_face_orientation_i.hpp
//   patch_face_orientation.cpp
#ifndef patch_face_orientation_hpp_
#define patch_face_orientation_hpp_
#include "tensor.hpp"
#include "indirect_primitive_patch.hpp"
namespace mousse
{
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
inline bool contiguous<patchFaceOrientation>()
{
  return true;
}
}  // namespace mousse
#include "patch_face_orientation_i.hpp"
#endif
