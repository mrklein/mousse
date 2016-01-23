// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_shape_recognition.hpp"
#include "label_list.hpp"
#include "cell_modeller.hpp"
namespace mousse
{
cellShape extrudedTriangleCellShape
(
  const label cellIndex,
  const labelList& faceLabels,
  const faceList& faces,
  const labelList& owner,
  const labelList& neighbour,
  const label pointOffset,
  faceList& frontAndBackFaces
)
{
  static const cellModel* prismModelPtr_ = NULL;
  if (!prismModelPtr_)
  {
    prismModelPtr_ = cellModeller::lookup("prism");
  }
  const cellModel& prism = *prismModelPtr_;
  // Checking
  if (faceLabels.size() != 3)
  {
    FATAL_ERROR_IN
    (
      "extrudedTriangleCellShape(const label cellIndex, "
      "const labelList& faceLabels, const faceList& faces, "
      "const labelList& owner, const labelList& neighbour, "
      "const label pointOffset, faceList& frontAndBackFaces)"
    )
    << "Trying to create a triangle with " << faceLabels.size()
    << " faces"
    << abort(FatalError);
  }
  // make a list of outward-pointing faces
  labelListList localFaces{3};
  FOR_ALL(faceLabels, faceI)
  {
    const label curFaceLabel = faceLabels[faceI];
    const face& curFace = faces[curFaceLabel];
    if (curFace.size() != 2)
    {
      FATAL_ERROR_IN
      (
        "extrudedTriangleCellShape(const label cellIndex, "
        "const labelList& faceLabels, const faceList& faces, "
        "const labelList& owner, const labelList& neighbour, "
        "const label pointOffset, faceList& frontAndBackFaces)"
      )
      << "face " << curFaceLabel
      << "does not have 2 vertices. Number of vertices: " << curFace
      << abort(FatalError);
    }
    if (owner[curFaceLabel] == cellIndex)
    {
      localFaces[faceI] = curFace;
    }
    else if (neighbour[curFaceLabel] == cellIndex)
    {
      // Reverse the face.  Note: it is necessary to reverse by
      // hand to preserve connectivity of a 2-D mesh.
      //
      localFaces[faceI].setSize(curFace.size());
      FOR_ALL_REVERSE(curFace, i)
      {
        localFaces[faceI][curFace.size() - i - 1] = curFace[i];
      }
    }
    else
    {
      FATAL_ERROR_IN
      (
        "extrudedTriangleCellShape(const label cellIndex, "
        "const labelList& faceLabels, const faceList& faces, "
        "const labelList& owner, const labelList& neighbour, "
        "const label pointOffset, faceList& frontAndBackFaces)"
      )
      << "face " << curFaceLabel
      << " does not belong to cell " << cellIndex
      << ". Face owner: " << owner[curFaceLabel] << " neighbour: "
      << neighbour[curFaceLabel]
      << abort(FatalError);
    }
  }
  // Create a label list for the model
  if (localFaces[0][1] == localFaces[1][0])
  {
    // Set front and back plane faces
    labelList missingPlaneFace{3};
    // front plane
    missingPlaneFace[0] = localFaces[0][0];
    missingPlaneFace[1] = localFaces[1][1];
    missingPlaneFace[2] = localFaces[0][1];
    frontAndBackFaces[2*cellIndex] = face(missingPlaneFace);
    // back plane
    missingPlaneFace[0] = localFaces[0][0] + pointOffset;
    missingPlaneFace[1] = localFaces[0][1] + pointOffset;
    missingPlaneFace[2] = localFaces[1][1] + pointOffset;
    frontAndBackFaces[2*cellIndex + 1] = face(missingPlaneFace);
    // make a cell
    labelList cellShapeLabels(6);
    cellShapeLabels[0] = localFaces[0][0];
    cellShapeLabels[1] = localFaces[0][1];
    cellShapeLabels[2] = localFaces[1][1];
    cellShapeLabels[3] = localFaces[0][0] + pointOffset;
    cellShapeLabels[4] = localFaces[0][1] + pointOffset;
    cellShapeLabels[5] = localFaces[1][1] + pointOffset;
    return cellShape(prism, cellShapeLabels);
  }
  else if (localFaces[0][1] == localFaces[2][0])
  {
    // Set front and back plane faces
    labelList missingPlaneFace(3);
    // front plane
    missingPlaneFace[0] = localFaces[0][0];
    missingPlaneFace[1] = localFaces[2][1];
    missingPlaneFace[2] = localFaces[0][1];
    frontAndBackFaces[2*cellIndex] = face(missingPlaneFace);
    // back plane
    missingPlaneFace[0] = localFaces[0][0] + pointOffset;
    missingPlaneFace[1] = localFaces[0][1] + pointOffset;
    missingPlaneFace[2] = localFaces[2][1] + pointOffset;
    frontAndBackFaces[2*cellIndex + 1] = face(missingPlaneFace);
    // make a cell
    labelList cellShapeLabels(6);
    cellShapeLabels[0] = localFaces[0][0];
    cellShapeLabels[1] = localFaces[0][1];
    cellShapeLabels[2] = localFaces[2][1];
    cellShapeLabels[3] = localFaces[0][0] + pointOffset;
    cellShapeLabels[4] = localFaces[0][1] + pointOffset;
    cellShapeLabels[5] = localFaces[2][1] + pointOffset;
    return cellShape(prism, cellShapeLabels);
  }
  else
  {
    FATAL_ERROR_IN
    (
      "extrudedTriangleCellShape(const label cellIndex, "
      "const labelList& faceLabels, const faceList& faces, "
      "const labelList& owner, const labelList& neighbour, "
      "const label pointOffset, faceList& frontAndBackFaces)"
    )
    << "Problem with edge matching. Edges: " << localFaces
    << abort(FatalError);
  }
  // Return added to keep compiler happy
  return cellShape{prism, labelList(0)};
}
}  // namespace mousse
