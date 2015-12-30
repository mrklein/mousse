// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "m_grid_gen_gamg_agglomeration.hpp"
#include "fv_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "processor_ldu_interface.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(MGridGenGAMGAgglomeration, 0);
  addToRunTimeSelectionTable
  (
    GAMGAgglomeration,
    MGridGenGAMGAgglomeration,
    lduMesh
  );
}
// Private Member Functions 
void mousse::MGridGenGAMGAgglomeration::swap
(
  const lduInterfacePtrsList& interfaces,
  const labelUList& cellValues,
  PtrList<labelList>& nbrValues
) const
{
  // Initialise transfer of restrict addressing on the interface
  forAll(interfaces, inti)
  {
    if (interfaces.set(inti))
    {
      interfaces[inti].initInternalFieldTransfer
      (
        Pstream::nonBlocking,
        cellValues
      );
    }
  }
  if (Pstream::parRun())
  {
    Pstream::waitRequests();
  }
  // Get the interface agglomeration
  nbrValues.setSize(interfaces.size());
  forAll(interfaces, inti)
  {
    if (interfaces.set(inti))
    {
      nbrValues.set
      (
        inti,
        new labelList
        (
          interfaces[inti].internalFieldTransfer
          (
            Pstream::nonBlocking,
            cellValues
          )
        )
      );
    }
  }
}
void mousse::MGridGenGAMGAgglomeration::getNbrAgglom
(
  const lduAddressing& addr,
  const lduInterfacePtrsList& interfaces,
  const PtrList<labelList>& nbrGlobalAgglom,
  labelList& cellToNbrAgglom
) const
{
  cellToNbrAgglom.setSize(addr.size());
  cellToNbrAgglom = -1;
  forAll(interfaces, inti)
  {
    if (interfaces.set(inti))
    {
      if (isA<processorLduInterface>(interfaces[inti]))
      {
        const processorLduInterface& pldui =
          refCast<const processorLduInterface>(interfaces[inti]);
        if (pldui.myProcNo() > pldui.neighbProcNo())
        {
          const labelUList& faceCells =
            interfaces[inti].faceCells();
          const labelList& nbrData = nbrGlobalAgglom[inti];
          forAll(faceCells, i)
          {
            cellToNbrAgglom[faceCells[i]] = nbrData[i];
          }
        }
      }
    }
  }
}
void mousse::MGridGenGAMGAgglomeration::detectSharedFaces
(
  const lduMesh& mesh,
  const labelList& value,
  labelHashSet& sharedFaces
) const
{
  const lduAddressing& addr = mesh.lduAddr();
  const labelUList& lower = addr.lowerAddr();
  const labelUList& upper = addr.upperAddr();
  sharedFaces.clear();
  sharedFaces.resize(addr.lowerAddr().size()/100);
  // Detect any faces inbetween same value
  forAll(lower, faceI)
  {
    label lowerData = value[lower[faceI]];
    label upperData = value[upper[faceI]];
    if (lowerData != -1 && lowerData == upperData)
    {
      sharedFaces.insert(faceI);
    }
  }
}
// Constructors 
mousse::MGridGenGAMGAgglomeration::MGridGenGAMGAgglomeration
(
  const lduMesh& mesh,
  const dictionary& controlDict
)
:
  GAMGAgglomeration(mesh, controlDict),
  fvMesh_(refCast<const fvMesh>(mesh))
{
  // Min, max size of agglomerated cells
  label minSize(readLabel(controlDict.lookup("minSize")));
  label maxSize(readLabel(controlDict.lookup("maxSize")));
  // Number of iterations applied to improve agglomeration consistency across
  // processor boundaries
  label nProcConsistencyIter
  (
    readLabel(controlDict.lookup("nProcConsistencyIter"))
  );
  // Start geometric agglomeration from the cell volumes and areas of the mesh
  scalarField* VPtr = const_cast<scalarField*>(&fvMesh_.cellVolumes());
  scalarField magFaceAreas(sqrt(3.0)*mag(fvMesh_.faceAreas()));
  SubField<scalar> magSf(magFaceAreas, fvMesh_.nInternalFaces());
  scalarField* magSfPtr = const_cast<scalarField*>
  (
    &magSf.operator const scalarField&()
  );
  // Create the boundary area cell field
  scalarField* magSbPtr(new scalarField(fvMesh_.nCells(), 0));
  {
    scalarField& magSb = *magSbPtr;
    const labelList& own = fvMesh_.faceOwner();
    const vectorField& Sf = fvMesh_.faceAreas();
    forAll(Sf, facei)
    {
      if (!fvMesh_.isInternalFace(facei))
      {
        magSb[own[facei]] += mag(Sf[facei]);
      }
    }
  }
  // Agglomerate until the required number of cells in the coarsest level
  // is reached
  label nCreatedLevels = 0;
  while (nCreatedLevels < maxLevels_ - 1)
  {
    label nCoarseCells = -1;
    tmp<labelField> finalAgglomPtr = agglomerate
    (
      nCoarseCells,
      minSize,
      maxSize,
      meshLevel(nCreatedLevels).lduAddr(),
      *VPtr,
      *magSfPtr,
      *magSbPtr
    );
    // Adjust weights only
    for (int i=0; i<nProcConsistencyIter; i++)
    {
      const lduMesh& mesh = meshLevel(nCreatedLevels);
      const lduAddressing& addr = mesh.lduAddr();
      const lduInterfacePtrsList interfaces = mesh.interfaces();
      const labelField& agglom = finalAgglomPtr();
      // Global nubmering
      const globalIndex globalNumbering(nCoarseCells);
      labelField globalAgglom(addr.size());
      forAll(agglom, cellI)
      {
        globalAgglom[cellI] = globalNumbering.toGlobal(agglom[cellI]);
      }
      // Get the interface agglomeration
      PtrList<labelList> nbrGlobalAgglom;
      swap(interfaces, globalAgglom, nbrGlobalAgglom);
      // Get the interface agglomeration on a cell basis (-1 for all
      // other cells)
      labelList cellToNbrAgglom;
      getNbrAgglom(addr, interfaces, nbrGlobalAgglom, cellToNbrAgglom);
      // Mark all faces inbetween cells with same nbragglomeration
      labelHashSet sharedFaces(addr.size()/100);
      detectSharedFaces(mesh, cellToNbrAgglom, sharedFaces);
      //- Note: in-place update of weights is more effective it seems?
      //        Should not be. fluke?
      //scalarField weights(*faceWeightsPtr);
      scalarField weights = *magSfPtr;
      forAllConstIter(labelHashSet, sharedFaces, iter)
      {
        label faceI= iter.key();
        weights[faceI] *= 2.0;
      }
      // Redo the agglomeration using the new weights
      finalAgglomPtr = agglomerate
      (
        nCoarseCells,
        minSize,
        maxSize,
        meshLevel(nCreatedLevels).lduAddr(),
        *VPtr,
        weights,
        *magSbPtr
      );
    }
    if (continueAgglomerating(nCoarseCells))
    {
      nCells_[nCreatedLevels] = nCoarseCells;
      restrictAddressing_.set(nCreatedLevels, finalAgglomPtr);
    }
    else
    {
      break;
    }
    agglomerateLduAddressing(nCreatedLevels);
    // Agglomerate the cell volumes field for the next level
    {
      scalarField* aggVPtr
      (
        new scalarField(meshLevels_[nCreatedLevels].size())
      );
      // Restrict but no parallel agglomeration (not supported)
      restrictField(*aggVPtr, *VPtr, nCreatedLevels, false);
      if (nCreatedLevels)
      {
        delete VPtr;
      }
      VPtr = aggVPtr;
    }
    // Agglomerate the face areas field for the next level
    {
      scalarField* aggMagSfPtr
      (
        new scalarField
        (
          meshLevels_[nCreatedLevels].upperAddr().size(),
          0
        )
      );
      restrictFaceField(*aggMagSfPtr, *magSfPtr, nCreatedLevels);
      if (nCreatedLevels)
      {
        delete magSfPtr;
      }
      magSfPtr = aggMagSfPtr;
    }
    // Agglomerate the cell boundary areas field for the next level
    {
      scalarField* aggMagSbPtr
      (
        new scalarField(meshLevels_[nCreatedLevels].size())
      );
      // Restrict but no parallel agglomeration (not supported)
      restrictField(*aggMagSbPtr, *magSbPtr, nCreatedLevels, false);
      delete magSbPtr;
      magSbPtr = aggMagSbPtr;
    }
    nCreatedLevels++;
  }
  // Shrink the storage of the levels to those created
  compactLevels(nCreatedLevels);
  // Delete temporary geometry storage
  if (nCreatedLevels)
  {
    delete VPtr;
    delete magSfPtr;
  }
  delete magSbPtr;
}
