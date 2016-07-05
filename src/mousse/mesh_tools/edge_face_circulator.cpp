// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_face_circulator.hpp"
#include "primitive_mesh.hpp"


// Static Data Members
const mousse::edgeFaceCirculator mousse::edgeFaceCirculator::endConstIter
{
  *reinterpret_cast<primitiveMesh*>(0),       // primitiveMesh
  -1,                                         // faceLabel
  false,                                      // ownerSide
  -1,                                         // index
  false                                       // isBoundaryEdge
};
