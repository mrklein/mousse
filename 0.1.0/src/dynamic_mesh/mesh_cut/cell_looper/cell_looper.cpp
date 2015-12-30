// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_looper.hpp"
#include "poly_mesh.hpp"
#include "list_ops.hpp"
#include "mesh_tools.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(cellLooper, 0);
  defineRunTimeSelectionTable(cellLooper, word);
}
mousse::autoPtr<mousse::cellLooper> mousse::cellLooper::New
(
  const word& type,
  const polyMesh& mesh
)
{
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(type);
  if (cstrIter == wordConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "cellLooper::New(const word&, const polyMesh&)"
    )   << "Unknown set type "
      << type << nl << nl
      << "Valid cellLooper types : " << endl
      << wordConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<cellLooper>(cstrIter()(mesh));
}
// Private Member Functions 
// Get faces (on cell) connected to vertI which are not using edgeI
mousse::labelList mousse::cellLooper::getVertFacesNonEdge
(
  const label cellI,
  const label edgeI,
  const label vertI
) const
{
  // Get faces connected to startEdge
  label face0, face1;
  meshTools::getEdgeFaces(mesh(), cellI, edgeI, face0, face1);
  const labelList& pFaces = mesh().pointFaces()[vertI];
  labelList vertFaces(pFaces.size());
  label vertFaceI = 0;
  forAll(pFaces, pFaceI)
  {
    label faceI = pFaces[pFaceI];
    if
    (
      (faceI != face0)
    && (faceI != face1)
    && (meshTools::faceOnCell(mesh(), cellI, faceI))
    )
    {
      vertFaces[vertFaceI++] = faceI;
    }
  }
  vertFaces.setSize(vertFaceI);
  return vertFaces;
}
// Get first edge connected to vertI and on faceI
mousse::label mousse::cellLooper::getFirstVertEdge
(
  const label faceI,
  const label vertI
) const
{
  const labelList& fEdges = mesh().faceEdges()[faceI];
  forAll(fEdges, fEdgeI)
  {
    label edgeI = fEdges[fEdgeI];
    const edge& e = mesh().edges()[edgeI];
    if ((e.start() == vertI) || (e.end() == vertI))
    {
      return edgeI;
    }
  }
  FatalErrorIn
  (
    "getFirstVertEdge(const label, const label)"
  )   << "Can not find edge on face " << faceI
    << " using vertex " << vertI
    << abort(FatalError);
  return -1;
}
// Get edges (on cell) connected to vertI which are not on faceI
mousse::labelList mousse::cellLooper::getVertEdgesNonFace
(
  const label cellI,
  const label faceI,
  const label vertI
) const
{
  const labelList& exclEdges = mesh().faceEdges()[faceI];
  const labelList& pEdges = mesh().pointEdges()[vertI];
  labelList vertEdges(pEdges.size());
  label vertEdgeI = 0;
  forAll(pEdges, pEdgeI)
  {
    label edgeI = pEdges[pEdgeI];
    if
    (
      (findIndex(exclEdges, edgeI) == -1)
    && meshTools::edgeOnCell(mesh(), cellI, edgeI)
    )
    {
      vertEdges[vertEdgeI++] = edgeI;
    }
  }
  vertEdges.setSize(vertEdgeI);
  return vertEdges;
}
// Return edge from cellEdges that is most perpendicular
// to refinement direction.
mousse::label mousse::cellLooper::getMisAlignedEdge
(
  const vector& refDir,
  const label cellI
) const
{
  const labelList& cEdges = mesh().cellEdges()[cellI];
  label cutEdgeI = -1;
  scalar maxCos = -GREAT;
  forAll(cEdges, cEdgeI)
  {
    label edgeI = cEdges[cEdgeI];
    scalar cosAngle = mag(refDir & meshTools::normEdgeVec(mesh(), edgeI));
    if (cosAngle > maxCos)
    {
      maxCos = cosAngle;
      cutEdgeI = edgeI;
    }
  }
  return cutEdgeI;
}
// Constructors 
mousse::cellLooper::cellLooper(const polyMesh& mesh)
:
  edgeVertex(mesh)
{}
// Destructor 
mousse::cellLooper::~cellLooper()
{}
