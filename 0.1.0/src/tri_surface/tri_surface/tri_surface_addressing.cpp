// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "hash_table.hpp"
#include "sortable_list.hpp"
#include "transform.hpp"
#include "patch_tools.hpp"
// Private Member Functions 
void mousse::triSurface::calcSortedEdgeFaces() const
{
  if (sortedEdgeFacesPtr_)
  {
    FatalErrorIn("triSurface::calcSortedEdgeFaces()")
      << "sortedEdgeFacesPtr_ already set"
      << abort(FatalError);
  }
  const labelListList& eFaces = edgeFaces();
  sortedEdgeFacesPtr_ = new labelListList(eFaces.size());
  labelListList& sortedEdgeFaces = *sortedEdgeFacesPtr_;
  sortedEdgeFaces = PatchTools::sortedEdgeFaces(*this);
}
void mousse::triSurface::calcEdgeOwner() const
{
  if (edgeOwnerPtr_)
  {
    FatalErrorIn("triSurface::calcEdgeOwner()")
      << "edgeOwnerPtr_ already set"
      << abort(FatalError);
  }
  // create the owner list
  edgeOwnerPtr_ = new labelList(nEdges());
  labelList& edgeOwner = *edgeOwnerPtr_;
  forAll(edges(), edgeI)
  {
    const edge& e = edges()[edgeI];
    const labelList& myFaces = edgeFaces()[edgeI];
    if (myFaces.size() == 1)
    {
      edgeOwner[edgeI] = myFaces[0];
    }
    else
    {
      // Find the first face whose vertices are aligned with the edge.
      // (in case of multiply connected edge the best we can do)
      edgeOwner[edgeI] = -1;
      forAll(myFaces, i)
      {
        const labelledTri& f = localFaces()[myFaces[i]];
        if
        (
          ((f[0] == e.start()) && (f[1] == e.end()))
        || ((f[1] == e.start()) && (f[2] == e.end()))
        || ((f[2] == e.start()) && (f[0] == e.end()))
        )
        {
          edgeOwner[edgeI] = myFaces[i];
          break;
        }
      }
      if (edgeOwner[edgeI] == -1)
      {
        FatalErrorIn("triSurface::calcEdgeOwner()")
          << "Edge " << edgeI << " vertices:" << e
          << " is used by faces " << myFaces
          << " vertices:"
          << UIndirectList<labelledTri>(localFaces(), myFaces)()
          << " none of which use the edge vertices in the same order"
          << nl << "I give up" << abort(FatalError);
      }
    }
  }
}
