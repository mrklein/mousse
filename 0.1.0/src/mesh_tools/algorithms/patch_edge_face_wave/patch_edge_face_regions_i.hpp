// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
// Constructors 
inline mousse::patchEdgeFaceRegions::patchEdgeFaceRegions()
{}
inline mousse::patchEdgeFaceRegions::patchEdgeFaceRegions
(
  const labelList& regions
)
:
  regions_(regions)
{}
inline mousse::patchEdgeFaceRegions::patchEdgeFaceRegions
(
  const labelPair& regions
)
:
  regions_(2)
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
  (
    faceInfo.regions_[index],
    faceInfo.regions_[f.fcIndex(index)]
  );
  if (!sameOrientation)
  {
    orientedInfo.flip();
  }
  if (!faceInfo.valid(td))
  {
    FATAL_ERROR_IN("patchEdgeFaceRegions::updateEdge(..)")
      << "problem." << abort(FatalError);
  }
  if ((findIndex(orientedInfo, -1) != -1) || (findIndex(regions_, -1) != -1))
  {
    // Blocked edge/face
    return false;
  }
  bool changed = false;
  regions_.setSize(orientedInfo.size(), labelMax);
  FOR_ALL(orientedInfo, i)
  {
    if (orientedInfo[i] != -1 && orientedInfo[i] < regions_[i])
    {
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
  edge orientedInfo(edgeInfo.regions_[0], edgeInfo.regions_[1]);
  if (!sameOrientation)
  {
    orientedInfo.flip();
  }
  if (!edgeInfo.valid(td))
  {
    FATAL_ERROR_IN("patchEdgeFaceRegions::updateEdge(..)")
      << "problem." << abort(FatalError);
  }
  if ((findIndex(orientedInfo, -1) != -1) || (findIndex(regions_, -1) != -1))
  {
    // Blocked edge/face
    return false;
  }
  bool changed = false;
  regions_.setSize(orientedInfo.size(), labelMax);
  FOR_ALL(orientedInfo, i)
  {
    if (orientedInfo[i] != -1 && orientedInfo[i] < regions_[i])
    {
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
  edge orientedInfo
  (
    edgeInfo.regions_[0],
    edgeInfo.regions_[1]
  );
  if (!sameOrientation)
  {
    orientedInfo.flip();
  }
  if (!edgeInfo.valid(td))
  {
    FATAL_ERROR_IN("patchEdgeFaceRegions::updateFace(..)")
      << "problem." << abort(FatalError);
  }
  if ((findIndex(orientedInfo, -1) != -1) || (findIndex(regions_, -1) != -1))
  {
    // Blocked edge/face
    return false;
  }
  bool changed = false;
  // Scale if needed
  regions_.setSize(f.size(), labelMax);
  if (orientedInfo[0] < regions_[index0])
  {
    regions_[index0] = orientedInfo[0];
    changed = true;
  }
  if (orientedInfo[1] < regions_[index1])
  {
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
