// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_tools.hpp"
#include "packed_bool_list.hpp"
#include "bound_box.hpp"

// Finds area, starting at faceI, delimited by borderEdge.
// Marks all visited faces (from face-edge-face walk) with currentZone.
template
<
  class BoolListType,
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
void
mousse::PatchTools::markZone
(
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
  const BoolListType& borderEdge,
  const label faceI,
  const label currentZone,
  labelList&  faceZone
)
{
  const labelListList& faceEdges = p.faceEdges();
  const labelListList& edgeFaces = p.edgeFaces();
  // List of faces whose faceZone has been set.
  labelList changedFaces(1, faceI);
  while (true)
  {
    // Pick up neighbours of changedFaces
    DynamicList<label> newChangedFaces{2*changedFaces.size()};
    FOR_ALL(changedFaces, i)
    {
      label faceI = changedFaces[i];
      const labelList& fEdges = faceEdges[faceI];
      FOR_ALL(fEdges, fEdgeI)
      {
        label edgeI = fEdges[fEdgeI];
        if (!borderEdge[edgeI])
        {
          const labelList& eFaceLst = edgeFaces[edgeI];
          FOR_ALL(eFaceLst, j)
          {
            label nbrFaceI = eFaceLst[j];
            if (faceZone[nbrFaceI] == -1)
            {
              faceZone[nbrFaceI] = currentZone;
              newChangedFaces.append(nbrFaceI);
            }
            else if (faceZone[nbrFaceI] != currentZone)
            {
              FATAL_ERROR_IN
              (
                "PatchTools::markZone"
                "("
                  "const PrimitivePatch<Face, FaceList, "
                    "PointField, PointType>& p,"
                  "const BoolListType& borderEdge,"
                  "const label faceI,"
                  "const label currentZone,"
                  "labelList&  faceZone"
                ")"
              )
              << "Zones " << faceZone[nbrFaceI]
              << " at face " << nbrFaceI
              << " connects to zone " << currentZone
              << " at face " << faceI
              << abort(FatalError);
            }
          }
        }
      }
    }
    if (newChangedFaces.empty())
    {
      break;
    }
    // transfer from dynamic to normal list
    changedFaces.transfer(newChangedFaces);
  }
}


// Finds areas delimited by borderEdge (or 'real' edges).
// Fills faceZone accordingly
template
<
  class BoolListType,
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
mousse::label
mousse::PatchTools::markZones
(
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
  const BoolListType& borderEdge,
  labelList& faceZone
)
{
  faceZone.setSize(p.size());
  faceZone = -1;
  label zoneI = 0;
  for (label startFaceI = 0; startFaceI < faceZone.size();)
  {
    // Find next non-visited face
    for (; startFaceI < faceZone.size(); ++startFaceI)
    {
      if (faceZone[startFaceI] == -1)
      {
        faceZone[startFaceI] = zoneI;
        markZone(p, borderEdge, startFaceI, zoneI, faceZone);
        zoneI++;
        break;
      }
    }
  }
  return zoneI;
}


// Finds areas delimited by borderEdge (or 'real' edges).
// Fills faceZone accordingly
template
<
  class BoolListType,
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
void
mousse::PatchTools::subsetMap
(
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
  const BoolListType& includeFaces,
  labelList& pointMap,
  labelList& faceMap
)
{
  label faceI  = 0;
  label pointI = 0;
  const List<Face>& localFaces = p.localFaces();
  faceMap.setSize(localFaces.size());
  pointMap.setSize(p.nPoints());
  boolList pointHad{pointMap.size(), false};
  FOR_ALL(p, oldFaceI)
  {
    if (includeFaces[oldFaceI])
    {
      // Store new faces compact
      faceMap[faceI++] = oldFaceI;
      // Renumber labels for face
      const Face& f = localFaces[oldFaceI];
      FOR_ALL(f, fp)
      {
        const label ptLabel = f[fp];
        if (!pointHad[ptLabel])
        {
          pointHad[ptLabel]  = true;
          pointMap[pointI++] = ptLabel;
        }
      }
    }
  }
  // Trim
  faceMap.setSize(faceI);
  pointMap.setSize(pointI);
}


template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
void mousse::PatchTools::calcBounds
(
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
  boundBox& bb,
  label& nPoints
)
{
  // Unfortunately nPoints constructs meshPoints() so do compact version
  // ourselves
  const PointField& points = p.points();
  PackedBoolList pointIsUsed{points.size()};
  nPoints = 0;
  bb = boundBox::invertedBox;
  FOR_ALL(p, faceI)
  {
    const Face& f = p[faceI];
    FOR_ALL(f, fp)
    {
      label pointI = f[fp];
      if (pointIsUsed.set(pointI, 1u))
      {
        bb.min() = ::mousse::min(bb.min(), points[pointI]);
        bb.max() = ::mousse::max(bb.max(), points[pointI]);
        nPoints++;
      }
    }
  }
}
