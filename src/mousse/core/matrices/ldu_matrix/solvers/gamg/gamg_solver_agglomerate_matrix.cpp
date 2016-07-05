// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_solver.hpp"
#include "gamg_interface_field.hpp"
#include "processor_ldu_interface_field.hpp"
#include "processor_gamg_interface_field.hpp"
#include "sub_list.hpp"
#include "uindirect_list.hpp"


// Private Member Functions
void mousse::GAMGSolver::agglomerateMatrix
(
  const label fineLevelIndex,
  const lduMesh& coarseMesh,
  const lduInterfacePtrsList& coarseMeshInterfaces
)
{
  // Get fine matrix
  const lduMatrix& fineMatrix = matrixLevel(fineLevelIndex);
  if (UPstream::myProcNo(fineMatrix.mesh().comm()) != -1) {
    const label nCoarseFaces = agglomeration_.nFaces(fineLevelIndex);
    const label nCoarseCells = agglomeration_.nCells(fineLevelIndex);
    // Set the coarse level matrix
    matrixLevels_.set
    (
      fineLevelIndex,
      new lduMatrix{coarseMesh}
    );
    lduMatrix& coarseMatrix = matrixLevels_[fineLevelIndex];
    // Coarse matrix diagonal initialised by restricting the finer mesh
    // diagonal. Note that we size with the cached coarse nCells and not
    // the actual coarseMesh size since this might be dummy when processor
    // agglomerating.
    scalarField& coarseDiag = coarseMatrix.diag(nCoarseCells);
    agglomeration_.restrictField
    (
      coarseDiag,
      fineMatrix.diag(),
      fineLevelIndex,
      false               // no processor agglomeration
    );
    // Get reference to fine-level interfaces
    const lduInterfaceFieldPtrsList& fineInterfaces =
      interfaceLevel(fineLevelIndex);
    // Create coarse-level interfaces
    primitiveInterfaceLevels_.set
    (
      fineLevelIndex,
      new PtrList<lduInterfaceField>{fineInterfaces.size()}
    );
    PtrList<lduInterfaceField>& coarsePrimInterfaces =
      primitiveInterfaceLevels_[fineLevelIndex];
    interfaceLevels_.set
    (
      fineLevelIndex,
      new lduInterfaceFieldPtrsList{fineInterfaces.size()}
    );
    lduInterfaceFieldPtrsList& coarseInterfaces =
      interfaceLevels_[fineLevelIndex];
    // Set coarse-level boundary coefficients
    interfaceLevelsBouCoeffs_.set
    (
      fineLevelIndex,
      new FieldField<Field, scalar>{fineInterfaces.size()}
    );
    FieldField<Field, scalar>& coarseInterfaceBouCoeffs =
      interfaceLevelsBouCoeffs_[fineLevelIndex];
    // Set coarse-level internal coefficients
    interfaceLevelsIntCoeffs_.set
    (
      fineLevelIndex,
      new FieldField<Field, scalar>{fineInterfaces.size()}
    );
    FieldField<Field, scalar>& coarseInterfaceIntCoeffs =
      interfaceLevelsIntCoeffs_[fineLevelIndex];
    // Add the coarse level
    agglomerateInterfaceCoefficients
    (
      fineLevelIndex,
      coarseMeshInterfaces,
      coarsePrimInterfaces,
      coarseInterfaces,
      coarseInterfaceBouCoeffs,
      coarseInterfaceIntCoeffs
    );
    // Get face restriction map for current level
    const labelList& faceRestrictAddr =
      agglomeration_.faceRestrictAddressing(fineLevelIndex);
    const boolList& faceFlipMap =
      agglomeration_.faceFlipMap(fineLevelIndex);
    // Check if matrix is asymetric and if so agglomerate both upper
    // and lower coefficients ...
    if (fineMatrix.hasLower()) {
      // Get off-diagonal matrix coefficients
      const scalarField& fineUpper = fineMatrix.upper();
      const scalarField& fineLower = fineMatrix.lower();
      // Coarse matrix upper coefficients. Note passed in size
      scalarField& coarseUpper = coarseMatrix.upper(nCoarseFaces);
      scalarField& coarseLower = coarseMatrix.lower(nCoarseFaces);
      FOR_ALL(faceRestrictAddr, fineFacei) {
        label cFace = faceRestrictAddr[fineFacei];
        if (cFace >= 0) {
          // Check the orientation of the fine-face relative to the
          // coarse face it is being agglomerated into
          if (!faceFlipMap[fineFacei]) {
            coarseUpper[cFace] += fineUpper[fineFacei];
            coarseLower[cFace] += fineLower[fineFacei];
          } else {
            coarseUpper[cFace] += fineLower[fineFacei];
            coarseLower[cFace] += fineUpper[fineFacei];
          }
        } else {
          // Add the fine face coefficients into the diagonal.
          coarseDiag[-1 - cFace] += fineUpper[fineFacei] + fineLower[fineFacei];
        }
      }
    } else // ... Otherwise it is symmetric so agglomerate just the upper
    {
      // Get off-diagonal matrix coefficients
      const scalarField& fineUpper = fineMatrix.upper();
      // Coarse matrix upper coefficients
      scalarField& coarseUpper = coarseMatrix.upper(nCoarseFaces);
      FOR_ALL(faceRestrictAddr, fineFacei) {
        label cFace = faceRestrictAddr[fineFacei];
        if (cFace >= 0) {
          coarseUpper[cFace] += fineUpper[fineFacei];
        } else {
          // Add the fine face coefficient into the diagonal.
          coarseDiag[-1 - cFace] += 2*fineUpper[fineFacei];
        }
      }
    }
  }
}


// Agglomerate only the interface coefficients.
void mousse::GAMGSolver::agglomerateInterfaceCoefficients
(
  const label fineLevelIndex,
  const lduInterfacePtrsList& coarseMeshInterfaces,
  PtrList<lduInterfaceField>& coarsePrimInterfaces,
  lduInterfaceFieldPtrsList& coarseInterfaces,
  FieldField<Field, scalar>& coarseInterfaceBouCoeffs,
  FieldField<Field, scalar>& coarseInterfaceIntCoeffs
) const
{
  // Get reference to fine-level interfaces
  const lduInterfaceFieldPtrsList& fineInterfaces =
    interfaceLevel(fineLevelIndex);
  // Get reference to fine-level boundary coefficients
  const FieldField<Field, scalar>& fineInterfaceBouCoeffs =
    interfaceBouCoeffsLevel(fineLevelIndex);
  // Get reference to fine-level internal coefficients
  const FieldField<Field, scalar>& fineInterfaceIntCoeffs =
    interfaceIntCoeffsLevel(fineLevelIndex);
  const labelListList& patchFineToCoarse =
    agglomeration_.patchFaceRestrictAddressing(fineLevelIndex);
  const labelList& nPatchFaces =
    agglomeration_.nPatchFaces(fineLevelIndex);
  // Add the coarse level
  FOR_ALL(fineInterfaces, inti) {
    if (fineInterfaces.set(inti)) {
      const GAMGInterface& coarseInterface =
        refCast<const GAMGInterface>
        (
          coarseMeshInterfaces[inti]
        );
      coarsePrimInterfaces.set
      (
        inti,
        GAMGInterfaceField::New
        (
          coarseInterface,
          fineInterfaces[inti]
        ).ptr()
      );
      coarseInterfaces.set
      (
        inti,
        &coarsePrimInterfaces[inti]
      );
      const labelList& faceRestrictAddressing = patchFineToCoarse[inti];
      coarseInterfaceBouCoeffs.set
      (
        inti,
        new scalarField{nPatchFaces[inti], 0.0}
      );
      agglomeration_.restrictField
      (
        coarseInterfaceBouCoeffs[inti],
        fineInterfaceBouCoeffs[inti],
        faceRestrictAddressing
      );
      coarseInterfaceIntCoeffs.set
      (
        inti,
        new scalarField{nPatchFaces[inti], 0.0}
      );
      agglomeration_.restrictField
      (
        coarseInterfaceIntCoeffs[inti],
        fineInterfaceIntCoeffs[inti],
        faceRestrictAddressing
      );
    }
  }
}


// Gather matrices.
// Note: matrices get constructed with dummy mesh
void mousse::GAMGSolver::gatherMatrices
(
  const labelList& procIDs,
  const lduMesh& dummyMesh,
  const label meshComm,
  const lduMatrix& mat,
  const FieldField<Field, scalar>& interfaceBouCoeffs,
  const FieldField<Field, scalar>& interfaceIntCoeffs,
  const lduInterfaceFieldPtrsList& interfaces,
  PtrList<lduMatrix>& otherMats,
  PtrList<FieldField<Field, scalar> >& otherBouCoeffs,
  PtrList<FieldField<Field, scalar> >& otherIntCoeffs,
  List<boolList>& otherTransforms,
  List<List<label> >& otherRanks
) const
{
  if (debug) {
    Pout << "GAMGSolver::gatherMatrices :"
      << " collecting matrices from procs:" << procIDs
      << " using comm:" << meshComm << endl;
  }
  if (Pstream::myProcNo(meshComm) == procIDs[0]) {
    // Master.
    otherMats.setSize(procIDs.size()-1);
    otherBouCoeffs.setSize(procIDs.size()-1);
    otherIntCoeffs.setSize(procIDs.size()-1);
    otherTransforms.setSize(procIDs.size()-1);
    otherRanks.setSize(procIDs.size()-1);
    for (label procI = 1; procI < procIDs.size(); procI++) {
      label otherI = procI-1;
      IPstream fromSlave
      {
        Pstream::scheduled,
        static_cast<int>(procIDs[procI]),
        0,          // bufSize
        Pstream::msgType(),
        meshComm
      };
      otherMats.set(otherI, new lduMatrix(dummyMesh, fromSlave));
      // Receive number of/valid interfaces
      boolList& procTransforms = otherTransforms[otherI];
      List<label>& procRanks = otherRanks[otherI];
      fromSlave >> procTransforms;
      fromSlave >> procRanks;
      // Size coefficients
      otherBouCoeffs.set
      (
        otherI,
        new FieldField<Field, scalar>{procRanks.size()}
      );
      otherIntCoeffs.set
      (
        otherI,
        new FieldField<Field, scalar>{procRanks.size()}
      );
      FOR_ALL(procRanks, intI) {
        if (procRanks[intI] != -1) {
          otherBouCoeffs[otherI].set
          (
            intI,
            new scalarField{fromSlave}
          );
          otherIntCoeffs[otherI].set
          (
            intI,
            new scalarField{fromSlave}
          );
        }
      }
    }
  } else {
    // Send to master
    // Count valid interfaces
    boolList procTransforms{interfaceBouCoeffs.size(), false};
    List<label> procRanks{interfaceBouCoeffs.size(), -1};
    FOR_ALL(interfaces, intI) {
      if (interfaces.set(intI)) {
        const processorLduInterfaceField& interface =
          refCast<const processorLduInterfaceField>
          (
            interfaces[intI]
          );
        procTransforms[intI] = interface.doTransform();
        procRanks[intI] = interface.rank();
      }
    }
    OPstream toMaster
    {
      Pstream::scheduled,
      static_cast<int>(procIDs[0]),
      0,
      Pstream::msgType(),
      meshComm
    };
    toMaster << mat << procTransforms << procRanks;
    FOR_ALL(procRanks, intI) {
      if (procRanks[intI] != -1) {
        toMaster
          << interfaceBouCoeffs[intI]
          << interfaceIntCoeffs[intI];
      }
    }
  }
}


void mousse::GAMGSolver::procAgglomerateMatrix
(
  // Agglomeration information
  const labelList& /*procAgglomMap*/,
  const List<label>& agglomProcIDs,
  const label levelI,
  // Resulting matrix
  autoPtr<lduMatrix>& allMatrixPtr,
  FieldField<Field, scalar>& allInterfaceBouCoeffs,
  FieldField<Field, scalar>& allInterfaceIntCoeffs,
  PtrList<lduInterfaceField>& allPrimitiveInterfaces,
  lduInterfaceFieldPtrsList& allInterfaces
) const
{
  const lduMatrix& coarsestMatrix = matrixLevels_[levelI];
  const lduInterfaceFieldPtrsList& coarsestInterfaces =
    interfaceLevels_[levelI];
  const FieldField<Field, scalar>& coarsestBouCoeffs =
    interfaceLevelsBouCoeffs_[levelI];
  const FieldField<Field, scalar>& coarsestIntCoeffs =
    interfaceLevelsIntCoeffs_[levelI];
  const lduMesh& coarsestMesh = coarsestMatrix.mesh();
  label coarseComm = coarsestMesh.comm();
  label oldWarn = UPstream::warnComm;
  UPstream::warnComm = coarseComm;
  // Gather all matrix coefficients onto agglomProcIDs[0]
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PtrList<lduMatrix> otherMats;
  PtrList<FieldField<Field, scalar>> otherBouCoeffs;
  PtrList<FieldField<Field, scalar>> otherIntCoeffs;
  List<boolList> otherTransforms;
  List<List<label> > otherRanks;
  gatherMatrices
  (
    agglomProcIDs,
    coarsestMesh,
    coarseComm,
    coarsestMatrix,
    coarsestBouCoeffs,
    coarsestIntCoeffs,
    coarsestInterfaces,
    otherMats,
    otherBouCoeffs,
    otherIntCoeffs,
    otherTransforms,
    otherRanks
  );
  if (Pstream::myProcNo(coarseComm) == agglomProcIDs[0]) {
    // Agglomerate all matrix
    // ~~~~~~~~~~~~~~~~~~~~~~
    //Pout<< "Own matrix:" << coarsestMatrix.info() << endl;
    //
    //FOR_ALL(otherMats, i)
    //{
    //    Pout<< "** otherMats " << i << " "
    //        << otherMats[i].info()
    //        << endl;
    //}
    //Pout<< endl;
    const lduMesh& allMesh = agglomeration_.meshLevel(levelI+1);
    const labelList& cellOffsets = agglomeration_.cellOffsets(levelI+1);
    const labelListList& faceMap = agglomeration_.faceMap(levelI+1);
    const labelListList& boundaryMap = agglomeration_.boundaryMap(levelI+1);
    const labelListListList& boundaryFaceMap =
      agglomeration_.boundaryFaceMap(levelI+1);
    allMatrixPtr.reset(new lduMatrix{allMesh});
    lduMatrix& allMatrix = allMatrixPtr();
    if (coarsestMatrix.hasDiag()) {
      scalarField& allDiag = allMatrix.diag();
      SubList<scalar>
      (
        allDiag,
        coarsestMatrix.diag().size()
      ).assign
      (
        coarsestMatrix.diag()
      );

      FOR_ALL(otherMats, i) {
        SubList<scalar>
        (
          allDiag,
          otherMats[i].diag().size(),
          cellOffsets[i+1]
        ).assign
        (
          otherMats[i].diag()
        );
      }
    }
    if (coarsestMatrix.hasLower()) {
      scalarField& allLower = allMatrix.lower();
      UIndirectList<scalar>
      (
        allLower,
        faceMap[0]
      ) = coarsestMatrix.lower();
      FOR_ALL(otherMats, i) {
        UIndirectList<scalar>
        (
          allLower,
          faceMap[i+1]
        ) = otherMats[i].lower();
      }
    }
    if (coarsestMatrix.hasUpper()) {
      scalarField& allUpper = allMatrix.upper();
      UIndirectList<scalar>
      (
        allUpper,
        faceMap[0]
      ) = coarsestMatrix.upper();
      FOR_ALL(otherMats, i) {
        UIndirectList<scalar>
        (
          allUpper,
          faceMap[i+1]
        ) = otherMats[i].upper();
      }
    }
    // Agglomerate interface fields and coefficients
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    lduInterfacePtrsList allMeshInterfaces = allMesh.interfaces();
    allInterfaceBouCoeffs.setSize(allMeshInterfaces.size());
    allInterfaceIntCoeffs.setSize(allMeshInterfaces.size());
    allPrimitiveInterfaces.setSize(allMeshInterfaces.size());
    allInterfaces.setSize(allMeshInterfaces.size());
    FOR_ALL(allMeshInterfaces, intI) {
      const lduInterface& patch = allMeshInterfaces[intI];
      label size = patch.faceCells().size();
      allInterfaceBouCoeffs.set(intI, new scalarField{size});
      allInterfaceIntCoeffs.set(intI, new scalarField{size});
    }
    labelList nBounFaces{allMeshInterfaces.size()};
    FOR_ALL(boundaryMap, procI) {
      const FieldField<Field, scalar>& procBouCoeffs
      {
        (procI == 0)
        ? coarsestBouCoeffs
        : otherBouCoeffs[procI-1]
      };
      const FieldField<Field, scalar>& procIntCoeffs
      {
        (procI == 0)
        ? coarsestIntCoeffs
        : otherIntCoeffs[procI-1]
      };
      const labelList& bMap = boundaryMap[procI];
      FOR_ALL(bMap, procIntI) {
        label allIntI = bMap[procIntI];
        if (allIntI != -1) {
          // So this boundary has been preserved. Copy
          // data across.
          if (!allInterfaces.set(allIntI)) {
            // Construct lduInterfaceField
            bool doTransform = false;
            int rank = -1;
            if (procI == 0) {
              const processorGAMGInterfaceField& procInt =
                refCast
                <
                  const processorGAMGInterfaceField
                >
                (
                  coarsestInterfaces[procIntI]
                );
              doTransform = procInt.doTransform();
              rank = procInt.rank();
            } else {
              doTransform =
                otherTransforms[procI-1][procIntI];
              rank = otherRanks[procI-1][procIntI];
            }
            allPrimitiveInterfaces.set
            (
              allIntI,
              GAMGInterfaceField::New
              (
                refCast<const GAMGInterface>
                (
                  allMeshInterfaces[allIntI]
                ),
                doTransform,
                rank
              ).ptr()
            );
            allInterfaces.set
            (
              allIntI,
              &allPrimitiveInterfaces[allIntI]
            );
          }
          // Map data from processor to complete mesh
          scalarField& allBou = allInterfaceBouCoeffs[allIntI];
          scalarField& allInt = allInterfaceIntCoeffs[allIntI];
          const labelList& map = boundaryFaceMap[procI][procIntI];
          const scalarField& procBou = procBouCoeffs[procIntI];
          const scalarField& procInt = procIntCoeffs[procIntI];
          FOR_ALL(map, i) {
            label allFaceI = map[i];
            if (allFaceI < 0) {
              FATAL_ERROR_IN("GAMGSolver::GAMGSolver()")
                << "problem." << abort(FatalError);
            }
            allBou[allFaceI] = procBou[i];
            allInt[allFaceI] = procInt[i];
          }
        } else if (procBouCoeffs.set(procIntI)) {
          // Boundary has become internal face
          const labelList& map = boundaryFaceMap[procI][procIntI];
          const scalarField& procBou = procBouCoeffs[procIntI];
          const scalarField& procInt = procIntCoeffs[procIntI];
          FOR_ALL(map, i) {
            if (map[i] >= 0) {
              label allFaceI = map[i];
              if (coarsestMatrix.hasUpper()) {
                allMatrix.upper()[allFaceI] = -procBou[i];
              }
              if (coarsestMatrix.hasLower()) {
                allMatrix.lower()[allFaceI] = -procInt[i];
              }
            } else {
              label allFaceI = -map[i]-1;
              if (coarsestMatrix.hasUpper()) {
                allMatrix.upper()[allFaceI] = -procInt[i];
              }
              if (coarsestMatrix.hasLower()) {
                allMatrix.lower()[allFaceI] = -procBou[i];
              }
            }
          }
        }
      }
    }
  }
  UPstream::warnComm = oldWarn;
}


void mousse::GAMGSolver::procAgglomerateMatrix
(
  const labelList& procAgglomMap,
  const List<label>& agglomProcIDs,
  const label levelI
)
{
  autoPtr<lduMatrix> allMatrixPtr;
  autoPtr<FieldField<Field, scalar>> allInterfaceBouCoeffs
  {
    new FieldField<Field, scalar>{0}
  };
  autoPtr<FieldField<Field, scalar>> allInterfaceIntCoeffs
  {
    new FieldField<Field, scalar>{0}
  };
  autoPtr<PtrList<lduInterfaceField>> allPrimitiveInterfaces
  {
    new PtrList<lduInterfaceField>{0}
  };
  autoPtr<lduInterfaceFieldPtrsList> allInterfaces
  {
    new lduInterfaceFieldPtrsList{0}
  };
  procAgglomerateMatrix
  (
    // Agglomeration information
    procAgglomMap,
    agglomProcIDs,
    levelI,
    // Resulting matrix
    allMatrixPtr,
    allInterfaceBouCoeffs(),
    allInterfaceIntCoeffs(),
    allPrimitiveInterfaces(),
    allInterfaces()
  );
  matrixLevels_.set(levelI, allMatrixPtr);
  interfaceLevelsBouCoeffs_.set(levelI, allInterfaceBouCoeffs);
  interfaceLevelsIntCoeffs_.set(levelI, allInterfaceIntCoeffs);
  primitiveInterfaceLevels_.set(levelI, allPrimitiveInterfaces);
  interfaceLevels_.set(levelI, allInterfaces);
}
