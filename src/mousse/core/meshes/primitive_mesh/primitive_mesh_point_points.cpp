// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"


// Private Member Functions 
void mousse::primitiveMesh::calcPointPoints() const
{
  if (debug) {
    Pout << "primitiveMesh::calcPointPoints() : "
      << "calculating pointPoints"
      << endl;
    if (debug == -1) {
      // For checking calls:abort so we can quickly hunt down
      // origin of call
      FATAL_ERROR_IN("primitiveMesh::calcPointPoints()")
        << abort(FatalError);
    }
  }
  // It is an error to attempt to recalculate pointPoints
  // if the pointer is already set
  if (ppPtr_) {
    FATAL_ERROR_IN("primitiveMesh::calcPointPoints() const")
      << "pointPoints already calculated"
      << abort(FatalError);
  } else {
    const edgeList& e = edges();
    const labelListList& pe = pointEdges();
    ppPtr_ = new labelListList{pe.size()};
    labelListList& pp = *ppPtr_;
    FOR_ALL(pe, pointI) {
      pp[pointI].setSize(pe[pointI].size());
      FOR_ALL(pe[pointI], ppi) {
        if (e[pe[pointI][ppi]].start() == pointI) {
          pp[pointI][ppi] = e[pe[pointI][ppi]].end();
        } else if (e[pe[pointI][ppi]].end() == pointI) {
          pp[pointI][ppi] = e[pe[pointI][ppi]].start();
        } else {
          FATAL_ERROR_IN("primitiveMesh::calcPointPoints() const")
            << "something wrong with edges"
            << abort(FatalError);
        }
      }
    }
  }
}


// Member Functions 
const mousse::labelListList& mousse::primitiveMesh::pointPoints() const
{
  if (!ppPtr_) {
    calcPointPoints();
  }
  return *ppPtr_;
}


const mousse::labelList& mousse::primitiveMesh::pointPoints
(
  const label pointI,
  DynamicList<label>& storage
) const
{
  if (hasPointPoints()) {
    return pointPoints()[pointI];
  } else {
    const edgeList& edges = this->edges();
    const labelList& pEdges = pointEdges()[pointI];
    storage.clear();
    if (pEdges.size() > storage.capacity()) {
      storage.setCapacity(pEdges.size());
    }
    FOR_ALL(pEdges, i) {
      storage.append(edges[pEdges[i]].otherVertex(pointI));
    }
    return storage;
  }
}


const mousse::labelList& mousse::primitiveMesh::pointPoints
(
  const label pointI
) const
{
  return pointPoints(pointI, labels_);
}

