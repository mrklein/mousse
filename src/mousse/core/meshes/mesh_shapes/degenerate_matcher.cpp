// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "degenerate_matcher.hpp"
#include "list_ops.hpp"


// Static Data Members
mousse::hexMatcher mousse::degenerateMatcher::hex;
mousse::wedgeMatcher mousse::degenerateMatcher::wedge;
mousse::prismMatcher mousse::degenerateMatcher::prism;
mousse::tetWedgeMatcher mousse::degenerateMatcher::tetWedge;
mousse::pyrMatcher mousse::degenerateMatcher::pyr;
mousse::tetMatcher mousse::degenerateMatcher::tet;


mousse::cellShape mousse::degenerateMatcher::match
(
  const faceList& faces,
  const labelList& owner,
  const label cellI,
  const labelList& cellFaces
)
{
  // Recognize in order of assumed occurrence.
  if (hex.matchShape(false, faces, owner, cellI, cellFaces)) {
    return cellShape(hex.model(), hex.vertLabels());
  } else if (tet.matchShape(false, faces, owner, cellI, cellFaces)) {
    return cellShape(tet.model(), tet.vertLabels());
  } else if (prism.matchShape(false, faces, owner, cellI, cellFaces)) {
    return cellShape(prism.model(), prism.vertLabels());
  } else if (pyr.matchShape(false, faces, owner, cellI, cellFaces)) {
    return cellShape(pyr.model(), pyr.vertLabels());
  } else if (wedge.matchShape(false, faces, owner, cellI, cellFaces)) {
    return cellShape(wedge.model(), wedge.vertLabels());
  } else if (tetWedge.matchShape(false, faces, owner, cellI, cellFaces)) {
    return cellShape(tetWedge.model(), tetWedge.vertLabels());
  } else {
    return cellShape(*(cellModeller::lookup(0)), labelList(0));
  }
}


mousse::cellShape mousse::degenerateMatcher::match(const faceList& faces)
{
  // Do as if single cell mesh; all faces are referenced by a single cell
  return match
  (
    faces,
    labelList(faces.size(), 0),    // cell 0 is owner of all faces
    0,                             // cell 0
    identity(faces.size())         // cell 0 consists of all faces
  );
}


mousse::cellShape mousse::degenerateMatcher::match(const cellShape& shape)
{
  return match(shape.collapsedFaces());
}


mousse::cellShape mousse::degenerateMatcher::match
(
  const primitiveMesh& mesh,
  const label cellI
)
{
  return match
  (
    mesh.faces(),
    mesh.faceOwner(),
    cellI,
    mesh.cells()[cellI]
  );
}
