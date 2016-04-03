#ifndef MESH_TOOLS_ALGORITHMS_PATCH_EDGE_FACE_WAVE_PATCH_EDGE_FACE_REGIONS_HPP_
#define MESH_TOOLS_ALGORITHMS_PATCH_EDGE_FACE_WAVE_PATCH_EDGE_FACE_REGIONS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchEdgeFaceRegions
// Description
//   Transport of regions for use in PatchEdgeFaceWave.
//   Set element to -1 to denote blocked.

#include "label_list.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "label_pair.hpp"
#include "poly_mesh.hpp"


namespace mousse {

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


// Constructors 
inline mousse::patchEdgeFaceRegions::patchEdgeFaceRegions()
{}


inline mousse::patchEdgeFaceRegions::patchEdgeFaceRegions
(
  const labelList& regions
)
:
  regions_{regions}
{}


inline mousse::patchEdgeFaceRegions::patchEdgeFaceRegions
(
  const labelPair& regions
)
:
  regions_{2}
{
  regions_[0] = regions[0];
  regions_[1] = regions[1];
}


// Member Functions 
inline const mousse::labelList& mousse::patchEdgeFaceRegions::regions() const
{
  return regions_;
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceRegions::valid(TrackingData&) const
{
  return regions_.size() && (findIndex(regions_, labelMax) == -1);
}


template<class Patch, class TrackingData>
inline void mousse::patchEdgeFaceRegions::transform
(
  const polyMesh&,
  const Patch&,
  const tensor& /*rotTensor*/,
  const scalar /*tol*/,
  TrackingData&
)
{}


template<class Patch, class TrackingData>
inline bool mousse::patchEdgeFaceRegions::updateEdge
(
  const polyMesh&,
  const Patch& patch,
  const label edgeI,
  const label faceI,
  const patchEdgeFaceRegions& faceInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  const face& f = patch.localFaces()[faceI];
  const edge& e = patch.edges()[edgeI];
  label index = findIndex(patch.faceEdges()[faceI], edgeI);
  bool sameOrientation = (f[index] == e.start());
  // Get information in edge-order
  edge orientedInfo
  {
    faceInfo.regions_[index],
    faceInfo.regions_[f.fcIndex(index)]
  };
  if (!sameOrientation) {
    orientedInfo.flip();
  }
  if (!faceInfo.valid(td)) {
    FATAL_ERROR_IN("patchEdgeFaceRegions::updateEdge(..)")
      << "problem." << abort(FatalError);
  }
  if ((findIndex(orientedInfo, -1) != -1) || (findIndex(regions_, -1) != -1)) {
    // Blocked edge/face
    return false;
  }
  bool changed = false;
  regions_.setSize(orientedInfo.size(), labelMax);
  FOR_ALL(orientedInfo, i) {
    if (orientedInfo[i] != -1 && orientedInfo[i] < regions_[i]) {
      regions_[i] = orientedInfo[i];
      changed = true;
    }
  }
  return changed;
}


template<class Patch, class TrackingData>
inline bool mousse::patchEdgeFaceRegions::updateEdge
(
  const polyMesh&,
  const Patch&,
  const patchEdgeFaceRegions& edgeInfo,
  const bool sameOrientation,
  const scalar /*tol*/,
  TrackingData& td
)
{
  // Get information in edge-order
  edge orientedInfo{edgeInfo.regions_[0], edgeInfo.regions_[1]};
  if (!sameOrientation) {
    orientedInfo.flip();
  }
  if (!edgeInfo.valid(td)) {
    FATAL_ERROR_IN("patchEdgeFaceRegions::updateEdge(..)")
      << "problem." << abort(FatalError);
  }
  if ((findIndex(orientedInfo, -1) != -1) || (findIndex(regions_, -1) != -1)) {
    // Blocked edge/face
    return false;
  }
  bool changed = false;
  regions_.setSize(orientedInfo.size(), labelMax);
  FOR_ALL(orientedInfo, i) {
    if (orientedInfo[i] != -1 && orientedInfo[i] < regions_[i]) {
      regions_[i] = orientedInfo[i];
      changed = true;
    }
  }
  return changed;
}


template<class Patch, class TrackingData>
inline bool mousse::patchEdgeFaceRegions::updateFace
(
  const polyMesh&,
  const Patch& patch,
  const label faceI,
  const label edgeI,
  const patchEdgeFaceRegions& edgeInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  const face& f = patch.localFaces()[faceI];
  const edge& e = patch.edges()[edgeI];
  // Find starting point of edge on face.
  label index0 = findIndex(patch.faceEdges()[faceI], edgeI);
  label index1 = f.fcIndex(index0);
  bool sameOrientation = (f[index0] == e.start());
  // Get information in face-order
  edge orientedInfo{edgeInfo.regions_[0], edgeInfo.regions_[1]};
  if (!sameOrientation) {
    orientedInfo.flip();
  }
  if (!edgeInfo.valid(td)) {
    FATAL_ERROR_IN("patchEdgeFaceRegions::updateFace(..)")
      << "problem." << abort(FatalError);
  }
  if ((findIndex(orientedInfo, -1) != -1) || (findIndex(regions_, -1) != -1)) {
    // Blocked edge/face
    return false;
  }
  bool changed = false;
  // Scale if needed
  regions_.setSize(f.size(), labelMax);
  if (orientedInfo[0] < regions_[index0]) {
    regions_[index0] = orientedInfo[0];
    changed = true;
  }
  if (orientedInfo[1] < regions_[index1]) {
    regions_[index1] = orientedInfo[1];
    changed = true;
  }
  return changed;
}


template<class TrackingData>
inline bool mousse::patchEdgeFaceRegions::equal
(
  const patchEdgeFaceRegions& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}


// Member Operators 
inline bool mousse::patchEdgeFaceRegions::operator==
(
  const mousse::patchEdgeFaceRegions& rhs
) const
{
  return regions() == rhs.regions();
}


inline bool mousse::patchEdgeFaceRegions::operator!=
(
  const mousse::patchEdgeFaceRegions& rhs
) const
{
  return !(*this == rhs);
}

#endif
