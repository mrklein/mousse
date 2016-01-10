// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_to_mesh0.hpp"
#include "tet_overlap_volume.hpp"
// Private Member Functions 
void mousse::meshToMesh0::calculateInverseDistanceWeights() const
{
  if (debug)
  {
    Info<< "meshToMesh0::calculateInverseDistanceWeights() : "
      << "calculating inverse distance weighting factors" << endl;
  }
  if (inverseDistanceWeightsPtr_)
  {
    FATAL_ERROR_IN("meshToMesh0::calculateInverseDistanceWeights()")
      << "weighting factors already calculated"
      << exit(FatalError);
  }
  //- Initialise overlap volume to zero
  V_ = 0.0;
  inverseDistanceWeightsPtr_ = new scalarListList(toMesh_.nCells());
  scalarListList& invDistCoeffs = *inverseDistanceWeightsPtr_;
  // get reference to source mesh data
  const labelListList& cc = fromMesh_.cellCells();
  const vectorField& centreFrom = fromMesh_.C().internalField();
  const vectorField& centreTo = toMesh_.C().internalField();
  FOR_ALL(cellAddressing_, celli)
  {
    if (cellAddressing_[celli] != -1)
    {
      const vector& target = centreTo[celli];
      scalar m = mag(target - centreFrom[cellAddressing_[celli]]);
      const labelList& neighbours = cc[cellAddressing_[celli]];
      // if the nearest cell is a boundary cell or there is a direct hit,
      // pick up the value
      label directCelli = -1;
      if (m < directHitTol || neighbours.empty())
      {
        directCelli = celli;
      }
      else
      {
        FOR_ALL(neighbours, ni)
        {
          scalar nm = mag(target - centreFrom[neighbours[ni]]);
          if (nm < directHitTol)
          {
            directCelli = neighbours[ni];
            break;
          }
        }
      }
      if (directCelli != -1)
      {
        // Direct hit
        invDistCoeffs[directCelli].setSize(1);
        invDistCoeffs[directCelli][0] = 1.0;
        V_ += fromMesh_.V()[cellAddressing_[directCelli]];
      }
      else
      {
        invDistCoeffs[celli].setSize(neighbours.size() + 1);
        // The first coefficient corresponds to the centre cell.
        // The rest is ordered in the same way as the cellCells list.
        scalar invDist = 1.0/m;
        invDistCoeffs[celli][0] = invDist;
        scalar sumInvDist = invDist;
        // now add the neighbours
        FOR_ALL(neighbours, ni)
        {
          invDist = 1.0/mag(target - centreFrom[neighbours[ni]]);
          invDistCoeffs[celli][ni + 1] = invDist;
          sumInvDist += invDist;
        }
        // divide by the total inverse-distance
        FOR_ALL(invDistCoeffs[celli], i)
        {
          invDistCoeffs[celli][i] /= sumInvDist;
        }
        V_ +=
          invDistCoeffs[celli][0]
         *fromMesh_.V()[cellAddressing_[celli]];
        for (label i = 1; i < invDistCoeffs[celli].size(); i++)
        {
          V_ +=
            invDistCoeffs[celli][i]*fromMesh_.V()[neighbours[i-1]];
        }
      }
    }
  }
}
void mousse::meshToMesh0::calculateInverseVolumeWeights() const
{
  if (debug)
  {
    Info<< "meshToMesh0::calculateInverseVolumeWeights() : "
      << "calculating inverse volume weighting factors" << endl;
  }
  if (inverseVolumeWeightsPtr_)
  {
    FATAL_ERROR_IN("meshToMesh0::calculateInverseVolumeWeights()")
      << "weighting factors already calculated"
      << exit(FatalError);
  }
  //- Initialise overlap volume to zero
  V_ = 0.0;
  inverseVolumeWeightsPtr_ = new scalarListList(toMesh_.nCells());
  scalarListList& invVolCoeffs = *inverseVolumeWeightsPtr_;
  const labelListList& cellToCell = cellToCellAddressing();
  tetOverlapVolume overlapEngine;
  FOR_ALL(cellToCell, celli)
  {
    const labelList& overlapCells = cellToCell[celli];
    if (overlapCells.size() > 0)
    {
      invVolCoeffs[celli].setSize(overlapCells.size());
      FOR_ALL(overlapCells, j)
      {
        label cellFrom = overlapCells[j];
        treeBoundBox bbFromMesh
        (
          pointField
          (
            fromMesh_.points(),
            fromMesh_.cellPoints()[cellFrom]
          )
        );
        scalar v = overlapEngine.cellCellOverlapVolumeMinDecomp
        (
          toMesh_,
          celli,
          fromMesh_,
          cellFrom,
          bbFromMesh
        );
        invVolCoeffs[celli][j] = v/toMesh_.V()[celli];
        V_ += v;
      }
    }
  }
}
void mousse::meshToMesh0::calculateCellToCellAddressing() const
{
  if (debug)
  {
    Info<< "meshToMesh0::calculateCellToCellAddressing() : "
      << "calculating cell to cell addressing" << endl;
  }
  if (cellToCellAddressingPtr_)
  {
    FATAL_ERROR_IN("meshToMesh0::calculateCellToCellAddressing()")
      << "addressing already calculated"
      << exit(FatalError);
  }
  //- Initialise overlap volume to zero
  V_ = 0.0;
  tetOverlapVolume overlapEngine;
  cellToCellAddressingPtr_ = new labelListList(toMesh_.nCells());
  labelListList& cellToCell = *cellToCellAddressingPtr_;
  FOR_ALL(cellToCell, iTo)
  {
    const labelList overLapCells =
      overlapEngine.overlappingCells(fromMesh_, toMesh_, iTo);
    if (overLapCells.size() > 0)
    {
      //Info << "To " << iTo << endl;
      //Info << "cellToCell " << overLapCells << endl;
      cellToCell[iTo].setSize(overLapCells.size());
      FOR_ALL(overLapCells, j)
      {
        cellToCell[iTo][j] = overLapCells[j];
        V_ += fromMesh_.V()[overLapCells[j]];
      }
    }
  }
}
const mousse::scalarListList& mousse::meshToMesh0::inverseDistanceWeights() const
{
  if (!inverseDistanceWeightsPtr_)
  {
    calculateInverseDistanceWeights();
  }
  return *inverseDistanceWeightsPtr_;
}
const mousse::scalarListList& mousse::meshToMesh0::inverseVolumeWeights() const
{
  if (!inverseVolumeWeightsPtr_)
  {
    calculateInverseVolumeWeights();
  }
  return *inverseVolumeWeightsPtr_;
}
const mousse::labelListList& mousse::meshToMesh0::cellToCellAddressing() const
{
  if (!cellToCellAddressingPtr_)
  {
    calculateCellToCellAddressing();
  }
  return *cellToCellAddressingPtr_;
}
