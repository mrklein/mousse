// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"


// Private Member Functions 
void mousse::primitiveMesh::calcCellCentresAndVols() const
{
  if (debug) {
    Pout << "primitiveMesh::calcCellCentresAndVols() : "
      << "Calculating cell centres and cell volumes"
      << endl;
  }
  // It is an error to attempt to recalculate cellCentres
  // if the pointer is already set
  if (cellCentresPtr_ || cellVolumesPtr_) {
    FATAL_ERROR_IN("primitiveMesh::calcCellCentresAndVols() const")
      << "Cell centres or cell volumes already calculated"
      << abort(FatalError);
  }
  // set the accumulated cell centre to zero vector
  cellCentresPtr_ = new vectorField{nCells()};
  vectorField& cellCtrs = *cellCentresPtr_;
  // Initialise cell volumes to 0
  cellVolumesPtr_ = new scalarField{nCells()};
  scalarField& cellVols = *cellVolumesPtr_;
  // Make centres and volumes
  makeCellCentresAndVols(faceCentres(), faceAreas(), cellCtrs, cellVols);
  if (debug) {
    Pout << "primitiveMesh::calcCellCentresAndVols() : "
      << "Finished calculating cell centres and cell volumes"
      << endl;
  }
}


void mousse::primitiveMesh::makeCellCentresAndVols
(
  const vectorField& fCtrs,
  const vectorField& fAreas,
  vectorField& cellCtrs,
  scalarField& cellVols
) const
{
  // Clear the fields for accumulation
  cellCtrs = vector::zero;
  cellVols = 0.0;
  const labelList& own = faceOwner();
  const labelList& nei = faceNeighbour();
  // first estimate the approximate cell centre as the average of
  // face centres
  vectorField cEst{nCells(), vector::zero};
  labelField nCellFaces{nCells(), 0};
  FOR_ALL(own, facei) {
    cEst[own[facei]] += fCtrs[facei];
    nCellFaces[own[facei]] += 1;
  }
  FOR_ALL(nei, facei) {
    cEst[nei[facei]] += fCtrs[facei];
    nCellFaces[nei[facei]] += 1;
  }
  FOR_ALL(cEst, celli) {
    cEst[celli] /= nCellFaces[celli];
  }
  FOR_ALL(own, facei) {
    // Calculate 3*face-pyramid volume
    scalar pyr3Vol =
      fAreas[facei] & (fCtrs[facei] - cEst[own[facei]]);
    // Calculate face-pyramid centre
    vector pc = (3.0/4.0)*fCtrs[facei] + (1.0/4.0)*cEst[own[facei]];
    // Accumulate volume-weighted face-pyramid centre
    cellCtrs[own[facei]] += pyr3Vol*pc;
    // Accumulate face-pyramid volume
    cellVols[own[facei]] += pyr3Vol;
  }
  FOR_ALL(nei, facei) {
    // Calculate 3*face-pyramid volume
    scalar pyr3Vol =
      fAreas[facei] & (cEst[nei[facei]] - fCtrs[facei]);
    // Calculate face-pyramid centre
    vector pc = (3.0/4.0)*fCtrs[facei] + (1.0/4.0)*cEst[nei[facei]];
    // Accumulate volume-weighted face-pyramid centre
    cellCtrs[nei[facei]] += pyr3Vol*pc;
    // Accumulate face-pyramid volume
    cellVols[nei[facei]] += pyr3Vol;
  }
  FOR_ALL(cellCtrs, celli) {
    if (mag(cellVols[celli]) > VSMALL) {
      cellCtrs[celli] /= cellVols[celli];
    } else {
      cellCtrs[celli] = cEst[celli];
    }
  }
  cellVols *= (1.0/3.0);
}


// Member Functions 
const mousse::vectorField& mousse::primitiveMesh::cellCentres() const
{
  if (!cellCentresPtr_) {
    calcCellCentresAndVols();
  }
  return *cellCentresPtr_;
}


const mousse::scalarField& mousse::primitiveMesh::cellVolumes() const
{
  if (!cellVolumesPtr_) {
    calcCellCentresAndVols();
  }
  return *cellVolumesPtr_;
}

