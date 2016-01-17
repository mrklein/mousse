// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "dynamic_list.hpp"
#include "list_ops.hpp"
// Private Member Functions 
void mousse::primitiveMesh::calcCellEdges() const
{
  // Loop through all faces and mark up cells with edges of the face.
  // Check for duplicates
  if (debug)
  {
    Pout<< "primitiveMesh::calcCellEdges() : "
      << "calculating cellEdges"
      << endl;
    if (debug == -1)
    {
      // For checking calls:abort so we can quickly hunt down
      // origin of call
      FATAL_ERROR_IN("primitiveMesh::calcCellEdges()")
        << abort(FatalError);
    }
  }
  // It is an error to attempt to recalculate cellEdges
  // if the pointer is already set
  if (cePtr_)
  {
    FATAL_ERROR_IN("primitiveMesh::calcCellEdges() const")
      << "cellEdges already calculated"
      << abort(FatalError);
  }
  else
  {
    // Set up temporary storage
    List<DynamicList<label, edgesPerCell_> > ce(nCells());
    // Get reference to faceCells and faceEdges
    const labelList& own = faceOwner();
    const labelList& nei = faceNeighbour();
    const labelListList& fe = faceEdges();
    // loop through the list again and add edges; checking for duplicates
    FOR_ALL(own, faceI)
    {
      DynamicList<label, edgesPerCell_>& curCellEdges = ce[own[faceI]];
      const labelList& curEdges = fe[faceI];
      FOR_ALL(curEdges, edgeI)
      {
        if (findIndex(curCellEdges, curEdges[edgeI]) == -1)
        {
          // Add the edge
          curCellEdges.append(curEdges[edgeI]);
        }
      }
    }
    FOR_ALL(nei, faceI)
    {
      DynamicList<label, edgesPerCell_>& curCellEdges = ce[nei[faceI]];
      const labelList& curEdges = fe[faceI];
      FOR_ALL(curEdges, edgeI)
      {
        if (findIndex(curCellEdges, curEdges[edgeI]) == -1)
        {
          // add the edge
          curCellEdges.append(curEdges[edgeI]);
        }
      }
    }
    cePtr_ = new labelListList(ce.size());
    labelListList& cellEdgeAddr = *cePtr_;
    // reset the size
    FOR_ALL(ce, cellI)
    {
      cellEdgeAddr[cellI].transfer(ce[cellI]);
    }
  }
}
// Member Functions 
const mousse::labelListList& mousse::primitiveMesh::cellEdges() const
{
  if (!cePtr_)
  {
    calcCellEdges();
  }
  return *cePtr_;
}
