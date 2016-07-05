// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mgridgen_gamg_agglomeration.hpp"
#include "fv_mesh.hpp"
#include "sync_tools.hpp"


// Private Member Functions 
void mousse::MGridGenGAMGAgglomeration::
makeCompactCellFaceAddressingAndFaceWeights
(
  const lduAddressing& fineAddressing,
  List<idxtype>& cellCells,
  List<idxtype>& cellCellOffsets,
  const scalarField& magSi,
  List<scalar>& faceWeights
)
{
  const label nFineCells = fineAddressing.size();
  const label nFineFaces = fineAddressing.upperAddr().size();
  const labelUList& upperAddr = fineAddressing.upperAddr();
  const labelUList& lowerAddr = fineAddressing.lowerAddr();
  // Number of neighbours for each cell
  labelList nNbrs{nFineCells, 0};
  FOR_ALL(upperAddr, facei) {
    nNbrs[upperAddr[facei]]++;
  }
  FOR_ALL(lowerAddr, facei) {
    nNbrs[lowerAddr[facei]]++;
  }
  // Set the sizes of the addressing and faceWeights arrays
  cellCellOffsets.setSize(nFineCells + 1);
  cellCells.setSize(2*nFineFaces);
  faceWeights.setSize(2*nFineFaces);
  cellCellOffsets[0] = 0;
  FOR_ALL(nNbrs, celli) {
    cellCellOffsets[celli+1] = cellCellOffsets[celli] + nNbrs[celli];
  }
  // reset the whole list to use as counter
  nNbrs = 0;
  FOR_ALL(upperAddr, facei) {
    label own = upperAddr[facei];
    label nei = lowerAddr[facei];
    label l1 = cellCellOffsets[own] + nNbrs[own]++;
    label l2 = cellCellOffsets[nei] + nNbrs[nei]++;
    cellCells[l1] = nei;
    cellCells[l2] = own;
    faceWeights[l1] = magSi[facei];
    faceWeights[l2] = magSi[facei];
  }
}


mousse::tmp<mousse::labelField> mousse::MGridGenGAMGAgglomeration::agglomerate
(
  label& nCoarseCells,
  const label minSize,
  const label maxSize,
  const lduAddressing& fineAddressing,
  const scalarField& V,
  const scalarField& magSf,
  const scalarField& magSb
)
{
  const label nFineCells = fineAddressing.size();
  // Compact addressing for cellCells
  List<idxtype> cellCells;
  List<idxtype> cellCellOffsets;
  // Face weights = face areas of the internal faces
  List<scalar> faceWeights;
  // Create the compact addressing for cellCells and faceWeights
  makeCompactCellFaceAddressingAndFaceWeights
  (
    fineAddressing,
    cellCells,
    cellCellOffsets,
    magSf,
    faceWeights
  );
  // agglomeration options.
  List<int> options{4, 0};
  options[0] = 4;                     // globular agglom
  options[1] = 6;                     // objective F3 and F2
  options[2] = 128;                   // debugging output level
  options[3] = fvMesh_.nGeometricD(); // Dimensionality of the grid
  // output: cell -> processor addressing
  List<int> finalAgglom{nFineCells};
  int nMoves = -1;
  MGridGen
  (
    nFineCells,
    cellCellOffsets.begin(),
    const_cast<scalar*>(V.begin()),
    const_cast<scalar*>(magSb.begin()),
    cellCells.begin(),
    faceWeights.begin(),
    minSize,
    maxSize,
    options.begin(),
    &nMoves,
    &nCoarseCells,
    finalAgglom.begin()
  );
  {
    label nNewCoarseCells = 0;
    labelList newRestrictAddr;
    bool ok =
      checkRestriction
      (
        newRestrictAddr,
        nNewCoarseCells,
        fineAddressing,
        finalAgglom,
        nCoarseCells
      );
    if (!ok) {
      nCoarseCells = nNewCoarseCells;
      finalAgglom.transfer(newRestrictAddr);
    }
  }
  return tmp<labelField>{new labelField{finalAgglom}};
}

