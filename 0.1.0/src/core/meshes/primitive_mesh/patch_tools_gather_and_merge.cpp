// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_tools.hpp"
#include "list_list_ops.hpp"
#include "merge_points.hpp"
#include "face.hpp"
#include "tri_face.hpp"
#include "list_list_ops.hpp"

// Member Functions 
template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
void mousse::PatchTools::gatherAndMerge
(
  const scalar mergeDist,
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
  Field<PointType>& mergedPoints,
  List<Face>& mergedFaces,
  labelList& pointMergeMap
)
{
  // Collect points from all processors
  labelList pointSizes;
  {
    List<Field<PointType>> gatheredPoints{Pstream::nProcs()};
    gatheredPoints[Pstream::myProcNo()] = p.points();
    Pstream::gatherList(gatheredPoints);
    if (Pstream::master())
    {
      pointSizes = ListListOps::subSizes
      (
        gatheredPoints,
        accessOp<Field<PointType> >()
      );
      mergedPoints = ListListOps::combine<Field<PointType> >
      (
        gatheredPoints,
        accessOp<Field<PointType> >()
      );
    }
  }
  // Collect faces from all processors and renumber using sizes of
  // gathered points
  {
    List<List<Face>> gatheredFaces{Pstream::nProcs()};
    gatheredFaces[Pstream::myProcNo()] = p;
    Pstream::gatherList(gatheredFaces);
    if (Pstream::master())
    {
      mergedFaces = static_cast<const List<Face>&>
      (
        ListListOps::combineOffset<List<Face>>
        (
          gatheredFaces,
          pointSizes,
          accessOp<List<Face> >(),
          offsetOp<Face>()
        )
      );
    }
  }
  if (Pstream::master())
  {
    Field<PointType> newPoints;
    labelList oldToNew;
    bool hasMerged = mergePoints
    (
      mergedPoints,
      mergeDist,
      false,                  // verbosity
      oldToNew,
      newPoints
    );
    if (hasMerged)
    {
      // Store point mapping
      pointMergeMap.transfer(oldToNew);
      // Copy points
      mergedPoints.transfer(newPoints);
      // Relabel faces
      List<Face>& faces = mergedFaces;
      FOR_ALL(faces, faceI)
      {
        inplaceRenumber(pointMergeMap, faces[faceI]);
      }
    }
  }
}
