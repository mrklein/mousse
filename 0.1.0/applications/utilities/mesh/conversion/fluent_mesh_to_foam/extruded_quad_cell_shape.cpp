// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_shape_recognition.hpp"
#include "label_list.hpp"


namespace mousse {

cellShape extrudedQuadCellShape
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
  static const cellModel* hexModelPtr_ = nullptr;
  if (!hexModelPtr_) {
    hexModelPtr_ = cellModeller::lookup("hex");
  }
  const cellModel& hex = *hexModelPtr_;
  // Checking
  if (faceLabels.size() != 4) {
    FATAL_ERROR_IN
    (
      "extrudedQuadCellShape(const label cellIndex, "
      "const labelList& faceLabels, const faceList& faces, "
      "const labelList& owner, const labelList& neighbour, "
      "const label pointOffset, faceList& frontAndBackFaces)"
    )
    << "Trying to create a quad with " << faceLabels.size() << " faces"
    << abort(FatalError);
  }
  // make a list of outward-pointing faces
  labelListList localFaces{4};
  FOR_ALL(faceLabels, faceI) {
    const label curFaceLabel = faceLabels[faceI];
    const face& curFace = faces[curFaceLabel];
    if (curFace.size() != 2) {
      FATAL_ERROR_IN
      (
        "extrudedQuadCellShape(const label cellIndex, "
        "const labelList& faceLabels, const faceList& faces, "
        "const labelList& owner, const labelList& neighbour, "
        "const label pointOffset, faceList& frontAndBackFaces)"
      )
      << "face " << curFaceLabel
      << "does not have 2 vertices. Number of vertices: " << curFace
      << abort(FatalError);
    }
    if (owner[curFaceLabel] == cellIndex) {
      localFaces[faceI] = curFace;
    } else if (neighbour[curFaceLabel] == cellIndex) {
      // Reverse the face.  Note: it is necessary to reverse by
      // hand to preserve connectivity of a 2-D mesh.
      //
      localFaces[faceI].setSize(curFace.size());
      FOR_ALL_REVERSE(curFace, i) {
        localFaces[faceI][curFace.size() - i - 1] = curFace[i];
      }
    } else {
      FATAL_ERROR_IN
      (
        "extrudedQuadCellShape(const label cellIndex, "
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
  // This is done by finding two edges that do not share any vertices.
  // Knowing the opposite pair of edges (with normals poining outward
  // is enough to make a cell
  if (localFaces[0][0] != localFaces[1][1]
      && localFaces[0][1] != localFaces[1][0]) {
    // Set front and back plane faces
    labelList missingPlaneFace{4};
    // front plane
    missingPlaneFace[0] = localFaces[0][0];
    missingPlaneFace[1] = localFaces[1][1];
    missingPlaneFace[2] = localFaces[1][0];
    missingPlaneFace[3] = localFaces[0][1];
    frontAndBackFaces[2*cellIndex] = face(missingPlaneFace);
    // back plane
    missingPlaneFace[0] = localFaces[0][0] + pointOffset;
    missingPlaneFace[1] = localFaces[0][1] + pointOffset;
    missingPlaneFace[2] = localFaces[1][0] + pointOffset;
    missingPlaneFace[3] = localFaces[1][1] + pointOffset;
    frontAndBackFaces[2*cellIndex + 1] = face(missingPlaneFace);
    // make a cell
    labelList cellShapeLabels{8};
    cellShapeLabels[0] = localFaces[0][0];
    cellShapeLabels[1] = localFaces[0][1];
    cellShapeLabels[2] = localFaces[1][0];
    cellShapeLabels[3] = localFaces[1][1];
    cellShapeLabels[4] = localFaces[0][0] + pointOffset;
    cellShapeLabels[5] = localFaces[0][1] + pointOffset;
    cellShapeLabels[6] = localFaces[1][0] + pointOffset;
    cellShapeLabels[7] = localFaces[1][1] + pointOffset;
    return {hex, cellShapeLabels};
  }
  else if (localFaces[0][0] != localFaces[2][1]
           && localFaces[0][1] != localFaces[2][0])
  {
    // Set front and back plane faces
    labelList missingPlaneFace{4};
    // front plane
    missingPlaneFace[0] = localFaces[0][0];
    missingPlaneFace[1] = localFaces[2][1];
    missingPlaneFace[2] = localFaces[2][0];
    missingPlaneFace[3] = localFaces[0][1];
    frontAndBackFaces[2*cellIndex] = face(missingPlaneFace);
    // back plane
    missingPlaneFace[0] = localFaces[0][0] + pointOffset;
    missingPlaneFace[1] = localFaces[0][1] + pointOffset;
    missingPlaneFace[2] = localFaces[2][0] + pointOffset;
    missingPlaneFace[3] = localFaces[2][1] + pointOffset;
    frontAndBackFaces[2*cellIndex + 1] = face(missingPlaneFace);
    // make a cell
    labelList cellShapeLabels(8);
    cellShapeLabels[0] = localFaces[0][0];
    cellShapeLabels[1] = localFaces[0][1];
    cellShapeLabels[2] = localFaces[2][0];
    cellShapeLabels[3] = localFaces[2][1];
    cellShapeLabels[4] = localFaces[0][0] + pointOffset;
    cellShapeLabels[5] = localFaces[0][1] + pointOffset;
    cellShapeLabels[6] = localFaces[2][0] + pointOffset;
    cellShapeLabels[7] = localFaces[2][1] + pointOffset;
    return {hex, cellShapeLabels};
  } else if (localFaces[0][0] != localFaces[3][1]
             && localFaces[0][1] != localFaces[3][0]) {
    // Set front and back plane faces
    labelList missingPlaneFace(4);
    // front plane
    missingPlaneFace[0] = localFaces[0][0];
    missingPlaneFace[1] = localFaces[3][1];
    missingPlaneFace[2] = localFaces[3][0];
    missingPlaneFace[3] = localFaces[0][1];
    frontAndBackFaces[2*cellIndex] = face(missingPlaneFace);
    // back plane
    missingPlaneFace[0] = localFaces[0][0] + pointOffset;
    missingPlaneFace[1] = localFaces[0][1] + pointOffset;
    missingPlaneFace[2] = localFaces[3][0] + pointOffset;
    missingPlaneFace[3] = localFaces[3][1] + pointOffset;
    frontAndBackFaces[2*cellIndex + 1] = face(missingPlaneFace);
    // make a cell
    labelList cellShapeLabels{8};
    cellShapeLabels[0] = localFaces[0][0];
    cellShapeLabels[1] = localFaces[0][1];
    cellShapeLabels[2] = localFaces[3][0];
    cellShapeLabels[3] = localFaces[3][1];
    cellShapeLabels[4] = localFaces[0][0] + pointOffset;
    cellShapeLabels[5] = localFaces[0][1] + pointOffset;
    cellShapeLabels[6] = localFaces[3][0] + pointOffset;
    cellShapeLabels[7] = localFaces[3][1] + pointOffset;
    return {hex, cellShapeLabels};
  } else {
    FATAL_ERROR_IN
    (
      "extrudedQuadCellShape(const label cellIndex, "
      "const labelList& faceLabels, const faceList& faces, "
      "const labelList& owner, const labelList& neighbour, "
      "const label pointOffset, faceList& frontAndBackFaces)"
    )
    << "Problem with edge matching. Edges: " << localFaces
    << abort(FatalError);
  }
  // Return added to keep compiler happy
  return {hex, labelList(0)};
}

}  // namespace mousse

