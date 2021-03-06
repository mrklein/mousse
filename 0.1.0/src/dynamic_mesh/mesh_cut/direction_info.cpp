// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "direction_info.hpp"
#include "poly_mesh.hpp"


// Private Member Functions 

// Find edge among edgeLabels that uses v0 and v1
mousse::label mousse::directionInfo::findEdge
(
  const primitiveMesh& mesh,
  const labelList& edgeLabels,
  const label v1,
  const label v0
)
{
  FOR_ALL(edgeLabels, edgeLabelI) {
    label edgeI = edgeLabels[edgeLabelI];
    if (mesh.edges()[edgeI] == edge(v0, v1)) {
      return edgeI;
    }
  }
  FATAL_ERROR_IN("directionInfo::findEdge")
    << "Cannot find an edge among " << edgeLabels << endl
    << "that uses vertices " << v0
    << " and " << v1
    << abort(FatalError);
  return -1;
}


mousse::label mousse::directionInfo::lowest
(
  const label size,
  const label a,
  const label b
)
{
  // Get next point
  label a1 = (a + 1) % size;
  if (a1 == b) {
    return a;
  } else {
    label b1 = (b + 1) % size;
    if (b1 != a) {
      FATAL_ERROR_IN("directionInfo::lowest")
        << "Problem : a:" << a << " b:" << b << " size:" << size
        << abort(FatalError);
    }
    return b;
  }
}


// Have edge on hex cell. Find corresponding edge on face. Return -1 if none
// found.
mousse::label mousse::directionInfo::edgeToFaceIndex
(
  const primitiveMesh& mesh,
  const label cellI,
  const label faceI,
  const label edgeI
)
{
  if ((edgeI < 0) || (edgeI >= mesh.nEdges())) {
    FATAL_ERROR_IN("directionInfo::edgeToFaceIndex")
      << "Illegal edge label:" << edgeI
      << " when projecting cut edge from cell " << cellI
      << " to face " << faceI
      << abort(FatalError);
  }
  const edge& e = mesh.edges()[edgeI];
  const face& f = mesh.faces()[faceI];
  // edgeI is either
  // - in faceI. Convert into index in face.
  // - connected (but not in) to face. Return -1.
  // - in face opposite faceI. Convert into index in face.
  label fpA = findIndex(f, e.start());
  label fpB = findIndex(f, e.end());
  if (fpA != -1) {
    if (fpB != -1) {
      return lowest(f.size(), fpA, fpB);
    } else {
      // e.start() in face, e.end() not
      return -1;
    }
  } else {
    if (fpB != -1) {
      // e.end() in face, e.start() not
      return -1;
    } else {
      // Both not in face.
      // e is on opposite face. Determine corresponding edge on this face:
      // - determine two faces using edge (one is the opposite face,
      //   one is 'side' face
      // - walk on both these faces to opposite edge
      // - check if this opposite edge is on faceI
      label f0I, f1I;
      meshTools::getEdgeFaces(mesh, cellI, edgeI, f0I, f1I);
      // Walk to opposite edge on face f0
      label edge0I = meshTools::walkFace(mesh, f0I, edgeI, e.start(), 2);
      // Check if edge on faceI.
      const edge& e0 = mesh.edges()[edge0I];
      fpA = findIndex(f, e0.start());
      fpB = findIndex(f, e0.end());
      if ((fpA != -1) && (fpB != -1)) {
        return lowest(f.size(), fpA, fpB);
      }
      // Face0 is doesn't have an edge on faceI (so must be the opposite
      // face) so try face1.
      // Walk to opposite edge on face f1
      label edge1I = meshTools::walkFace(mesh, f1I, edgeI, e.start(), 2);
      // Check if edge on faceI.
      const edge& e1 = mesh.edges()[edge1I];
      fpA = findIndex(f, e1.start());
      fpB = findIndex(f, e1.end());
      if ((fpA != -1) && (fpB != -1)) {
        return lowest(f.size(), fpA, fpB);
      }
      FATAL_ERROR_IN("directionInfo::edgeToFaceIndex")
        << "Found connected faces " << mesh.faces()[f0I] << " and "
        << mesh.faces()[f1I] << " sharing edge " << edgeI << endl
        << "But none seems to be connected to face " << faceI
        << " vertices:" << f
        << abort(FatalError);
      return -1;
    }
  }
}


// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::directionInfo& wDist
)
{
  if (os.format() == IOstream::ASCII) {
    os << wDist.index_ << wDist.n_;
  } else {
    os.write(reinterpret_cast<const char*>(&wDist.index_),
             sizeof(directionInfo));
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const directionInfo&)");
  return os;
}


mousse::Istream& mousse::operator>>(mousse::Istream& is, mousse::directionInfo& wDist)
{
  if (is.format() == IOstream::ASCII) {
    is >> wDist.index_ >> wDist.n_;
  } else {
    is.read(reinterpret_cast<char*>(&wDist.index_), sizeof(directionInfo));
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, directionInfo&)");
  return is;
}

