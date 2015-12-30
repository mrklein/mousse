// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "transform.hpp"
#include "oriented_surface.hpp"
// Private Member Functions 
inline mousse::patchFaceOrientation::patchFaceOrientation()
:
  flipStatus_(orientedSurface::UNVISITED)
{}
inline mousse::patchFaceOrientation::patchFaceOrientation
(
  const label flipStatus
)
:
  flipStatus_(flipStatus)
{}
// Member Functions 
inline mousse::label mousse::patchFaceOrientation::flipStatus() const
{
  return flipStatus_;
}
inline void mousse::patchFaceOrientation::flip()
{
  if (flipStatus_ == orientedSurface::NOFLIP)
  {
    flipStatus_ = orientedSurface::FLIP;
  }
  else if (flipStatus_ == orientedSurface::FLIP)
  {
    flipStatus_ = orientedSurface::NOFLIP;
  }
}
template<class TrackingData>
inline bool mousse::patchFaceOrientation::valid(TrackingData& td) const
{
  return flipStatus_ != orientedSurface::UNVISITED;
}
template<class TrackingData>
inline void mousse::patchFaceOrientation::transform
(
  const polyMesh& mesh,
  const indirectPrimitivePatch& patch,
  const tensor& rotTensor,
  const scalar tol,
  TrackingData& td
)
{}
template<class TrackingData>
inline bool mousse::patchFaceOrientation::updateEdge
(
  const polyMesh& mesh,
  const indirectPrimitivePatch& patch,
  const label edgeI,
  const label faceI,
  const patchFaceOrientation& faceInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (valid(td))
  {
    return false;
  }
  const face& f = patch.localFaces()[faceI];
  const edge& e = patch.edges()[edgeI];
  //Pout<< "Updating edge:" << edgeI << " verts:" << e << nl
  //    << "    start:" << patch.localPoints()[e[0]] << nl
  //    << "      end:" << patch.localPoints()[e[1]] << endl;
  patchFaceOrientation consistentInfo(faceInfo);
  // Check how edge relates to face
  if (f.edgeDirection(e) < 0)
  {
    // Create flipped version of faceInfo
    consistentInfo.flip();
  }
  operator=(consistentInfo);
  return true;
}
template<class TrackingData>
inline bool mousse::patchFaceOrientation::updateEdge
(
  const polyMesh& mesh,
  const indirectPrimitivePatch& patch,
  const patchFaceOrientation& edgeInfo,
  const bool sameOrientation,
  const scalar tol,
  TrackingData& td
)
{
  if (valid(td))
  {
    return false;
  }
  // Create (flipped/unflipped) version of edgeInfo
  patchFaceOrientation consistentInfo(edgeInfo);
  if (!sameOrientation)
  {
    consistentInfo.flip();
  }
  operator=(consistentInfo);
  return true;
}
template<class TrackingData>
inline bool mousse::patchFaceOrientation::updateFace
(
  const polyMesh& mesh,
  const indirectPrimitivePatch& patch,
  const label faceI,
  const label edgeI,
  const patchFaceOrientation& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (valid(td))
  {
    return false;
  }
  // Transfer flip to face
  const face& f = patch.localFaces()[faceI];
  const edge& e = patch.edges()[edgeI];
  //Pout<< "Updating face:" << faceI << nl
  //    << "    verts:" << f << nl
  //    << " with edge:" << edgeI << nl
  //    << "    start:" << patch.localPoints()[e[0]] << nl
  //    << "      end:" << patch.localPoints()[e[1]] << endl;
  // Create (flipped/unflipped) version of edgeInfo
  patchFaceOrientation consistentInfo(edgeInfo);
  if (f.edgeDirection(e) > 0)
  {
    consistentInfo.flip();
  }
  operator=(consistentInfo);
  return true;
}
template<class TrackingData>
inline bool mousse::patchFaceOrientation::equal
(
  const patchFaceOrientation& rhs,
  TrackingData& td
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
