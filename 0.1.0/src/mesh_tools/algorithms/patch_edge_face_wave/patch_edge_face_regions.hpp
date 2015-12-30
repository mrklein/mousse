// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchEdgeFaceRegions
// Description
//   Transport of regions for use in PatchEdgeFaceWave.
//   Set element to -1 to denote blocked.
// SourceFiles
//   patch_edge_face_regions_i.hpp
//   patch_edge_face_regions.cpp
#ifndef patch_edge_face_regions_hpp_
#define patch_edge_face_regions_hpp_
#include "label_list.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "label_pair.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class patchEdgeFaceRegions
{
  // Private data
    //- Region per point
    labelList regions_;
public:
  // Constructors
    //- Construct null
    inline patchEdgeFaceRegions();
    //- Construct from regions
    inline patchEdgeFaceRegions(const labelList&);
    //- Construct from regions (on edge)
    inline patchEdgeFaceRegions(const labelPair&);
  // Member Functions
    // Access
      inline const labelList& regions() const;
    // Needed by meshWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Apply rotation matrix
      template<class Patch, class TrackingData>
      inline void transform
      (
        const polyMesh& mesh,
        const Patch& patch,
        const tensor& rotTensor,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of face on edge
      template<class Patch, class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const Patch& patch,
        const label edgeI,
        const label faceI,
        const patchEdgeFaceRegions& faceInfo,
        const scalar tol,
        TrackingData& td
      );
      //- New information for edge (from e.g. coupled edge)
      template<class Patch, class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const Patch& patch,
        const patchEdgeFaceRegions& edgeInfo,
        const bool sameOrientation,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of edge on face.
      template<class Patch, class TrackingData>
      inline bool updateFace
      (
        const polyMesh& mesh,
        const Patch& patch,
        const label faceI,
        const label edgeI,
        const patchEdgeFaceRegions& edgeInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const patchEdgeFaceRegions&, TrackingData&) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const patchEdgeFaceRegions&) const;
    inline bool operator!=(const patchEdgeFaceRegions&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const patchEdgeFaceRegions&);
    friend Istream& operator>>(Istream&, patchEdgeFaceRegions&);
};
}  // namespace mousse
#include "patch_edge_face_regions_i.hpp"
#endif
