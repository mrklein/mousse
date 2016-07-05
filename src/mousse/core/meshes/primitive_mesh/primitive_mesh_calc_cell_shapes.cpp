// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "degenerate_matcher.hpp"


// Private Member Functions 
void mousse::primitiveMesh::calcCellShapes() const
{
  if (debug) {
    Pout << "primitiveMesh::calcCellShapes() : calculating cellShapes" << endl;
  }
  // It is an error to attempt to recalculate faceCells
  // if the pointer is already set
  if (cellShapesPtr_) {
    FATAL_ERROR_IN("primitiveMesh::calcCellShapes() const")
      << "cellShapes already calculated"
      << abort(FatalError);
  } else {
    cellShapesPtr_ = new cellShapeList{nCells()};
    cellShapeList& cellShapes = *cellShapesPtr_;
    FOR_ALL(cellShapes, celli) {
      cellShapes[celli] = degenerateMatcher::match(*this, celli);
    }
  }
}
