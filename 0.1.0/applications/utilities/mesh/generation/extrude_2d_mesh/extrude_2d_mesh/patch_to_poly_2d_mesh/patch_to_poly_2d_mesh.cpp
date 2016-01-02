// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_to_poly_2d_mesh.hpp"
#include "patch_tools.hpp"
// Private Member Functions 
void mousse::patchToPoly2DMesh::flipFaceOrder()
{
  const edgeList& edges = patch_.edges();
  const faceList& localFaces = patch_.localFaces();
  const labelList& meshPoints = patch_.meshPoints();
  Info<< "Flipping face order if necessary." << endl;
  forAll(edges, edgeI)
  {
    const edge& e = edges[edgeI];
    faces_[edgeI].setSize(2);
    label edgeOwner = owner_[edgeI];
    const face& f = localFaces[edgeOwner];
    label fp = findIndex(f, e[0]);
    if (f.nextLabel(fp) != e[1])
    {
      Info<< "Flipping face " << faces_[edgeI] << endl;
      faces_[edgeI][0] = meshPoints[e[1]];
      faces_[edgeI][1] = meshPoints[e[0]];
    }
    else
    {
      faces_[edgeI][0] = meshPoints[e[0]];
      faces_[edgeI][1] = meshPoints[e[1]];
    }
  }
}
void mousse::patchToPoly2DMesh::createNeighbours()
{
  const edgeList& edges = patch_.edges();
  const labelListList& edgeFaces = patch_.edgeFaces();
  Info<< "Calculating neighbours." << endl;
  forAll(edges, edgeI)
  {
    const labelList& eFaces = edgeFaces[edgeI];
    if (eFaces.size() == 2)
    {
      if (owner_[edgeI] == eFaces[0])
      {
        neighbour_[edgeI] = eFaces[1];
      }
      else
      {
        neighbour_[edgeI] = eFaces[0];
      }
    }
    else if (eFaces.size() == 1)
    {
      continue;
    }
    else
    {
      FatalErrorIn("polyMesh neighbour construction")
        << abort(FatalError);
    }
  }
}
mousse::labelList mousse::patchToPoly2DMesh::internalFaceOrder()
{
  const labelListList& faceEdges = patch_.faceEdges();
  labelList oldToNew(owner_.size(), -1);
  label newFaceI = 0;
  forAll(faceEdges, faceI)
  {
    const labelList& fEdges = faceEdges[faceI];
    // Neighbouring faces
    SortableList<label> nbr(fEdges.size(), -1);
    forAll(fEdges, feI)
    {
      if (fEdges[feI] < neighbour_.size())
      {
        // Internal edge. Get the face on other side.
        label nbrFaceI = neighbour_[fEdges[feI]];
        if (nbrFaceI == faceI)
        {
          nbrFaceI = owner_[fEdges[feI]];
        }
        if (faceI < nbrFaceI)
        {
          // faceI is master
          nbr[feI] = nbrFaceI;
        }
      }
    }
    nbr.sort();
    forAll(nbr, i)
    {
      if (nbr[i] != -1)
      {
        oldToNew[fEdges[nbr.indices()[i]]] = newFaceI++;
      }
    }
  }
  return oldToNew;
}
void mousse::patchToPoly2DMesh::addPatchFacesToFaces()
{
  const labelList& meshPoints = patch_.meshPoints();
  label offset = patch_.nInternalEdges();
  face f(2);
  forAll(patchNames_, patchI)
  {
    forAllConstIter(EdgeMap<label>, mapEdgesRegion_, eIter)
    {
      if (eIter() == patchI)
      {
        f[0] = meshPoints[eIter.key().start()];
        f[1] = meshPoints[eIter.key().end()];
        faces_[offset++] = f;
      }
    }
  }
  f.clear();
}
void mousse::patchToPoly2DMesh::addPatchFacesToOwner()
{
  const label nInternalEdges = patch_.nInternalEdges();
  const faceList& faces = patch_.faces();
  const label nExternalEdges = patch_.edges().size() - nInternalEdges;
  const labelList& meshPoints = patch_.meshPoints();
  // Reorder patch faces on owner list.
  labelList newOwner = owner_;
  label nMatched = 0;
  for
  (
    label bFaceI = nInternalEdges;
    bFaceI < faces_.size();
    ++bFaceI
  )
  {
    const face& e = faces_[bFaceI];
    bool matched = false;
    for
    (
      label bEdgeI = nInternalEdges;
      bEdgeI < faces_.size();
      ++bEdgeI
    )
    {
      if
      (
        e[0] == meshPoints[patch_.edges()[bEdgeI][0]]
      && e[1] == meshPoints[patch_.edges()[bEdgeI][1]]
      )
      {
        const face& f = faces[owner_[bEdgeI]];
        label fp = findIndex(f, e[0]);
        newOwner[bFaceI] = owner_[bEdgeI];
        if (f.nextLabel(fp) != e[1])
        {
          Info<< "Flipping" << endl;
          faces_[bFaceI][0] = e[1];
          faces_[bFaceI][1] = e[0];
        }
        nMatched++;
        matched = true;
      }
      else if
      (
        e[0] == meshPoints[patch_.edges()[bEdgeI][1]]
      && e[1] == meshPoints[patch_.edges()[bEdgeI][0]]
      )
      {
        Info<< "Warning: Wrong orientation." << endl;
        nMatched++;
        matched = true;
      }
    }
    if (!matched)
    {
      Info<< "No match for edge." << endl;
    }
  }
  if (nMatched != nExternalEdges)
  {
    Info<< "Number of matched edges, " << nMatched
      << ", does not match number of external edges, "
      << nExternalEdges << endl;
  }
  owner_ = newOwner.xfer();
}
void mousse::patchToPoly2DMesh::createPolyMeshComponents()
{
  flipFaceOrder();
  createNeighbours();
  // New function for returning a map of old faces to new faces.
  labelList oldToNew = internalFaceOrder();
  inplaceReorder(oldToNew, faces_);
  inplaceReorder(oldToNew, owner_);
  inplaceReorder(oldToNew, neighbour_);
  // Add patches.
  addPatchFacesToFaces();
  addPatchFacesToOwner();
}
// Constructors 
mousse::patchToPoly2DMesh::patchToPoly2DMesh
(
  const MeshedSurface<face>& patch,
  const wordList& patchNames,
  const labelList& patchSizes,
  const EdgeMap<label>& mapEdgesRegion
)
:
  patch_(patch),
  patchNames_(patchNames),
  patchSizes_(patchSizes),
  patchStarts_(patchNames.size(), 0),
  mapEdgesRegion_(mapEdgesRegion),
  points_(patch.points()),
  faces_(patch.nEdges()),
  owner_(PatchTools::edgeOwner(patch)),
  neighbour_(patch.nInternalEdges())
{}
// Destructor 
mousse::patchToPoly2DMesh::~patchToPoly2DMesh()
{}
// Member Functions 
void mousse::patchToPoly2DMesh::createMesh()
{
  for (label edgeI = 0; edgeI < patch_.nInternalEdges(); edgeI++)
  {
    if (patch_.edgeFaces()[edgeI].size() != 2)
    {
      FatalErrorIn("patchToPoly2DMesh::patchToPoly2DMesh(..)")
        << "internal edge:" << edgeI
        << " patch.edgeFaces()[edgeI]:" << patch_.edgeFaces()[edgeI]
        << abort(FatalError);
    }
  }
  for
  (
    label edgeI = patch_.nInternalEdges();
    edgeI < patch_.nEdges();
    edgeI++
  )
  {
    if (patch_.edgeFaces()[edgeI].size() != 1)
    {
      FatalErrorIn("patchToPoly2DMesh::patchToPoly2DMesh(..)")
        << "boundary edge:" << edgeI
        << " patch.edgeFaces()[edgeI]:" << patch_.edgeFaces()[edgeI]
        << abort(FatalError);
    }
  }
  createPolyMeshComponents();
  label startFace = patch_.nInternalEdges();
  forAll(patchNames_, patchI)
  {
    patchStarts_[patchI] = startFace;
    startFace += patchSizes_[patchI];
  }
}
