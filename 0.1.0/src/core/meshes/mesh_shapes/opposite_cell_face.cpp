// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell.hpp"
#include "opposite_face.hpp"
#include "bool_list.hpp"


// Member Functions 
mousse::label mousse::cell::opposingFaceLabel
(
  const label masterFaceLabel,
  const faceUList& meshFaces
) const
{
  // Algorithm:
  // Go through all the faces of the cell and find the one which
  // does not share a single vertex with the master face.  If there
  // are two or more such faces, return the first one and issue a
  // warning; if there is no opposite face, return -1;
  const face& masterFace = meshFaces[masterFaceLabel];
  const labelList& curFaceLabels = *this;
  label oppositeFaceLabel = -1;
  FOR_ALL(curFaceLabels, faceI) {
    // Compare the face with the master
    const face& curFace = meshFaces[curFaceLabels[faceI]];
    // Skip the master face
    if (curFaceLabels[faceI] != masterFaceLabel
        && curFace.size() == masterFace.size()) {
      bool sharedPoint = false;
      // Compare every vertex of the current face agains the
      // vertices of the master face
      FOR_ALL(curFace, pointI) {
        const label l = curFace[pointI];
        FOR_ALL(masterFace, masterPointI) {
          if (masterFace[masterPointI] == l) {
            sharedPoint = true;
            break;
          }
        }
        if (sharedPoint)
          break;
      }
      // If no points are shared, this is the opposite face
      if (!sharedPoint) {
        if (oppositeFaceLabel == -1) {
          // Found opposite face
          oppositeFaceLabel = curFaceLabels[faceI];
        } else {
          // There has already been an opposite face.
          // Non-prismatic cell
          Info << "Multiple faces not sharing vertex: "
            << oppositeFaceLabel << " and "
            << curFaceLabels[faceI] << endl;
          return -1;
        }
      }
    }
  }
  return oppositeFaceLabel;
}


mousse::oppositeFace mousse::cell::opposingFace
(
  const label masterFaceLabel,
  const faceUList& meshFaces
) const
{
  // Get the label of the opposite face
  label oppFaceLabel = opposingFaceLabel(masterFaceLabel, meshFaces);
  // If the opposing face is not found, return a failure
  if (oppFaceLabel < 0) {
    return oppositeFace(face(0), masterFaceLabel, oppFaceLabel);
  } else {
    // This is a prismatic cell.  Go through all the vertices of the master
    // face and find an edge going from the master face vertex to a slave
    // face vertex.  If all is OK, there should be only one such
    // edge for every master vertex and will provide te
    // master-to-slave vertex mapping.  Assemble the opposite face
    // in the same manner as the master.
    // Get reference to faces and prepare the return
    const face& masterFace = meshFaces[masterFaceLabel];
    const face& slaveFace = meshFaces[oppFaceLabel];
    // Get cell edges
    const edgeList e = edges(meshFaces);
    boolList usedEdges{e.size(), false};
    oppositeFace oppFace
    {
      face(masterFace.size()),
      masterFaceLabel,
      oppFaceLabel
    };
    FOR_ALL(masterFace, pointI) {
      // Go through the list of edges and find the edge from this vertex
      // to the slave face
      FOR_ALL(e, edgeI) {
        if (!usedEdges[edgeI]) {
          // Get the other vertex
          label otherVertex = e[edgeI].otherVertex(masterFace[pointI]);
          if (otherVertex != -1) {
            // Found an edge coming from this vertex.
            // Check all vertices of the slave to find out
            // if it exists.
            FOR_ALL(slaveFace, slavePointI) {
              if (slaveFace[slavePointI] == otherVertex) {
                usedEdges[edgeI] = true;
                oppFace[pointI] = otherVertex;
                break;
              }
            }
          }
        }
      }
    }
    return oppFace;
  }
}

