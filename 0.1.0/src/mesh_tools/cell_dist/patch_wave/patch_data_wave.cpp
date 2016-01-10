// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_data_wave.hpp"

// Private Member Functions 

// Set initial set of changed faces (= all wall faces)
template<class TransferType>
void mousse::patchDataWave<TransferType>::setChangedFaces
(
  const labelHashSet& patchIDs,
  labelList& changedFaces,
  List<TransferType>& faceDist
) const
{
  const polyMesh& mesh = cellDistFuncs::mesh();
  label nChangedFaces = 0;
  FOR_ALL(mesh.boundaryMesh(), patchI)
  {
    if (patchIDs.found(patchI))
    {
      const polyPatch& patch = mesh.boundaryMesh()[patchI];
      const Field<Type>& patchField = initialPatchValuePtrs_[patchI];
      FOR_ALL(patch.faceCentres(), patchFaceI)
      {
        label meshFaceI = patch.start() + patchFaceI;
        changedFaces[nChangedFaces] = meshFaceI;
        faceDist[nChangedFaces] =
          TransferType
          (
            patch.faceCentres()[patchFaceI],
            patchField[patchFaceI],
            0.0
          );
        nChangedFaces++;
      }
    }
  }
}


// Copy from MeshWave data into *this (distance) and field_ (transported data)
template<class TransferType>
mousse::label mousse::patchDataWave<TransferType>::getValues
(
  const MeshWave<TransferType>& waveInfo
)
{
  const polyMesh& mesh = cellDistFuncs::mesh();
  const List<TransferType>& cellInfo = waveInfo.allCellInfo();
  const List<TransferType>& faceInfo = waveInfo.allFaceInfo();
  label nIllegal = 0;
  // Copy cell values
  distance_.setSize(cellInfo.size());
  FOR_ALL(cellInfo, cellI)
  {
    const TransferType & wpn = cellInfo[cellI];
    scalar dist = wpn.distSqr();
    if (cellInfo[cellI].valid(waveInfo.data()))
    {
      distance_[cellI] = mousse::sqrt(dist);
      cellData_[cellI] = cellInfo[cellI].data();
    }
    else
    {
      // Illegal/unset value. What to do with data?
      // Note: mag for now. Should maybe be member of TransferType?
      distance_[cellI] = mag(dist);
      //cellData_[cellI] = point::max;
      cellData_[cellI] = cellInfo[cellI].data();
      nIllegal++;
    }
  }
  // Copy boundary values
  FOR_ALL(patchDistance_, patchI)
  {
    const polyPatch& patch = mesh.boundaryMesh()[patchI];
    // Allocate storage for patchDistance
    scalarField* patchFieldPtr = new scalarField(patch.size());
    patchDistance_.set(patchI, patchFieldPtr);
    scalarField& patchField = *patchFieldPtr;
    // Allocate storage for patchData
    Field<Type>* patchDataFieldPtr = new Field<Type>(patch.size());
    patchData_.set(patchI, patchDataFieldPtr);
    Field<Type>& patchDataField = *patchDataFieldPtr;
    // Copy distance and data
    FOR_ALL(patchField, patchFaceI)
    {
      label meshFaceI = patch.start() + patchFaceI;
      scalar dist = faceInfo[meshFaceI].distSqr();
      if (faceInfo[meshFaceI].valid(waveInfo.data()))
      {
        // Adding SMALL to avoid problems with /0 in the turbulence
        // models
        patchField[patchFaceI] = mousse::sqrt(dist) + SMALL;
        patchDataField[patchFaceI] = faceInfo[meshFaceI].data();
      }
      else
      {
        // Illegal/unset value. What to do with data?
        patchField[patchFaceI] = mag(dist);
        //patchDataField[patchFaceI] = point::max;
        patchDataField[patchFaceI] = faceInfo[meshFaceI].data();
        nIllegal++;
      }
    }
  }
  return nIllegal;
}


// Constructors 
// Construct from components
template<class TransferType>
mousse::patchDataWave<TransferType>::patchDataWave
(
  const polyMesh& mesh,
  const labelHashSet& patchIDs,
  const UPtrList<Field<Type> >& initialPatchValuePtrs,
  const bool correctWalls
)
:
  cellDistFuncs{mesh},
  patchIDs_{patchIDs},
  initialPatchValuePtrs_{initialPatchValuePtrs},
  correctWalls_{correctWalls},
  nUnset_{0},
  distance_{mesh.nCells()},
  patchDistance_{mesh.boundaryMesh().size()},
  cellData_{mesh.nCells()},
  patchData_{mesh.boundaryMesh().size()}
{
  patchDataWave<TransferType>::correct();
}


// Destructor 
template<class TransferType>
mousse::patchDataWave<TransferType>::~patchDataWave()
{}


// Member Functions 
// Correct for mesh geom/topo changes
template<class TransferType>
void mousse::patchDataWave<TransferType>::correct()
{
  //
  // Set initial changed faces: set TransferType for wall faces
  // to wall centre.
  //
  // Count walls
  label nWalls = sumPatchSize(patchIDs_);
  List<TransferType> faceDist(nWalls);
  labelList changedFaces(nWalls);
  setChangedFaces(patchIDs_, changedFaces, faceDist);
  //
  // Do calculate wall distance by 'growing' from faces.
  //
  MeshWave<TransferType> waveInfo
  (
    mesh(),
    changedFaces,
    faceDist,
    mesh().globalData().nTotalCells()+1 // max iterations
  );
  //
  // Copy distance into return field
  //
  nUnset_ = getValues(waveInfo);
  //
  // Correct wall cells for true distance
  //
  if (correctWalls_)
  {
    Map<label> nearestFace(2 * nWalls);
    // Get distance and indices of nearest face
    correctBoundaryFaceCells
    (
      patchIDs_,
      distance_,
      nearestFace
    );
    correctBoundaryPointCells
    (
      patchIDs_,
      distance_,
      nearestFace
    );
    // Transfer data from nearest face to cell
    const List<TransferType>& faceInfo = waveInfo.allFaceInfo();
    const labelList wallCells(nearestFace.toc());
    FOR_ALL(wallCells, wallCellI)
    {
      label cellI = wallCells[wallCellI];
      label faceI = nearestFace[cellI];
      cellData_[cellI] = faceInfo[faceI].data();
    }
  }
}
