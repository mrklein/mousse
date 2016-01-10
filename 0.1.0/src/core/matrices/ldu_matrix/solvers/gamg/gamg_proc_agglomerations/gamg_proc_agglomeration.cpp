// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_proc_agglomeration.hpp"
#include "gamg_agglomeration.hpp"
#include "ldu_mesh.hpp"
#include "global_index.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(GAMGProcAgglomeration, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(GAMGProcAgglomeration, GAMGAgglomeration);

}

// Protected Member Functions
void mousse::GAMGProcAgglomeration::printStats
(
  Ostream& os,
  GAMGAgglomeration& agglom
) const
{
  for (label levelI = 0; levelI <= agglom.size(); levelI++)
  {
    if (agglom.hasMeshLevel(levelI))
    {
      os  << agglom.meshLevel(levelI).info() << endl;
    }
    else
    {
      os  << "Level " << levelI << " has no fine mesh:" << endl;
    }
    if
    (
      levelI < agglom.restrictAddressing_.size()
    && agglom.restrictAddressing_.set(levelI)
    )
    {
      const labelList& cellRestrict =
        agglom.restrictAddressing(levelI);
      const labelList& faceRestrict =
        agglom.faceRestrictAddressing(levelI);
      os  << "Level " << levelI << " agglomeration:" << nl
        << "    nCoarseCells:" << agglom.nCells(levelI) << nl
        << "    nCoarseFaces:" << agglom.nFaces(levelI) << nl
        << "    cellRestriction:"
        << " size:" << cellRestrict.size()
        << " max:" << max(cellRestrict)
        << nl
        << "    faceRestriction:"
        << " size:" << faceRestrict.size()
        << " max:" << max(faceRestrict)
        << nl;
      const labelListList& patchFaceRestrict =
        agglom.patchFaceRestrictAddressing(levelI);
      FOR_ALL(patchFaceRestrict, i)
      {
        if (patchFaceRestrict[i].size())
        {
          const labelList& faceRestrict =
            patchFaceRestrict[i];
          os  << "        " << i
            << " size:" << faceRestrict.size()
            << " max:" << max(faceRestrict)
            << nl;
        }
      }
    }
    if
    (
      levelI < agglom.procCellOffsets_.size()
    && agglom.procCellOffsets_.set(levelI)
    )
    {
      os  << "    procCellOffsets:" << agglom.procCellOffsets_[levelI]
        << nl
        << "    procAgglomMap:" << agglom.procAgglomMap_[levelI]
        << nl
        << "    procIDs:" << agglom.agglomProcIDs_[levelI]
        << nl
        << "    comm:" << agglom.procCommunicator_[levelI]
        << endl;
    }
    os  << endl;
  }
  os  << endl;
}
mousse::labelListList mousse::GAMGProcAgglomeration::globalCellCells
(
  const lduMesh& mesh
)
{
  const lduAddressing& addr = mesh.lduAddr();
  lduInterfacePtrsList interfaces = mesh.interfaces();
  const label myProcID = Pstream::myProcNo(mesh.comm());
  globalIndex globalNumbering
  (
    addr.size(),
    Pstream::msgType(),
    mesh.comm(),
    Pstream::parRun()
  );
  labelList globalIndices(addr.size());
  FOR_ALL(globalIndices, cellI)
  {
    globalIndices[cellI] = globalNumbering.toGlobal(myProcID, cellI);
  }
  // Get the interface cells
  PtrList<labelList> nbrGlobalCells(interfaces.size());
  {
    // Initialise transfer of restrict addressing on the interface
    FOR_ALL(interfaces, inti)
    {
      if (interfaces.set(inti))
      {
        interfaces[inti].initInternalFieldTransfer
        (
          Pstream::nonBlocking,
          globalIndices
        );
      }
    }
    if (Pstream::parRun())
    {
      Pstream::waitRequests();
    }
    FOR_ALL(interfaces, inti)
    {
      if (interfaces.set(inti))
      {
        nbrGlobalCells.set
        (
          inti,
          new labelList
          (
            interfaces[inti].internalFieldTransfer
            (
              Pstream::nonBlocking,
              globalIndices
            )
          )
        );
      }
    }
  }
  // Scan the neighbour list to find out how many times the cell
  // appears as a neighbour of the face. Done this way to avoid guessing
  // and resizing list
  labelList nNbrs(addr.size(), 1);
  const labelUList& nbr = addr.upperAddr();
  const labelUList& own = addr.lowerAddr();
  {
    FOR_ALL(nbr, faceI)
    {
      nNbrs[nbr[faceI]]++;
      nNbrs[own[faceI]]++;
    }
    FOR_ALL(interfaces, inti)
    {
      if (interfaces.set(inti))
      {
        const labelUList& faceCells = interfaces[inti].faceCells();
        FOR_ALL(faceCells, i)
        {
          nNbrs[faceCells[i]]++;
        }
      }
    }
  }
  // Create cell-cells addressing
  labelListList cellCells(addr.size());
  FOR_ALL(cellCells, cellI)
  {
    cellCells[cellI].setSize(nNbrs[cellI], -1);
  }
  // Reset the list of number of neighbours to zero
  nNbrs = 0;
  // Scatter the neighbour faces
  FOR_ALL(nbr, faceI)
  {
    label c0 = own[faceI];
    label c1 = nbr[faceI];
    cellCells[c0][nNbrs[c0]++] = globalIndices[c1];
    cellCells[c1][nNbrs[c1]++] = globalIndices[c0];
  }
  FOR_ALL(interfaces, inti)
  {
    if (interfaces.set(inti))
    {
      const labelUList& faceCells = interfaces[inti].faceCells();
      FOR_ALL(faceCells, i)
      {
        label c0 = faceCells[i];
        cellCells[c0][nNbrs[c0]++] = nbrGlobalCells[inti][i];
      }
    }
  }
  FOR_ALL(cellCells, cellI)
  {
    mousse::stableSort(cellCells[cellI]);
  }
  // Replace the initial element (always -1) with the local cell
  FOR_ALL(cellCells, cellI)
  {
    cellCells[cellI][0] = globalIndices[cellI];
  }
  return cellCells;
}
bool mousse::GAMGProcAgglomeration::agglomerate
(
  const label fineLevelIndex,
  const labelList& procAgglomMap,
  const labelList& /*masterProcs*/,
  const List<label>& agglomProcIDs,
  const label procAgglomComm
)
{
  const lduMesh& levelMesh = agglom_.meshLevels_[fineLevelIndex];
  label levelComm = levelMesh.comm();
  if (Pstream::myProcNo(levelComm) != -1)
  {
    // Collect meshes and restrictAddressing onto master
    // Overwrites the fine mesh (meshLevels_[index-1]) and addressing
    // from fine mesh to coarse mesh (restrictAddressing_[index]).
    agglom_.procAgglomerateLduAddressing
    (
      levelComm,
      procAgglomMap,
      agglomProcIDs,
      procAgglomComm,
      fineLevelIndex               //fine level index
    );
    // Combine restrict addressing only onto master
    for
    (
      label levelI = fineLevelIndex+1;
      levelI < agglom_.meshLevels_.size();
      levelI++
    )
    {
      agglom_.procAgglomerateRestrictAddressing
      (
        levelComm,
        agglomProcIDs,
        levelI
      );
    }
    if (Pstream::myProcNo(levelComm) == agglomProcIDs[0])
    {
      // On master. Recreate coarse meshes from restrict addressing
      for
      (
        label levelI = fineLevelIndex;
        levelI < agglom_.meshLevels_.size();
        levelI++
      )
      {
        agglom_.agglomerateLduAddressing(levelI);
      }
    }
    else
    {
      // Agglomerated away. Clear mesh storage.
      for
      (
        label levelI = fineLevelIndex+1;
        levelI <= agglom_.size();
        levelI++
      )
      {
        agglom_.clearLevel(levelI);
      }
    }
  }
  // Should check!
  return true;
}

// Constructors
mousse::GAMGProcAgglomeration::GAMGProcAgglomeration
(
  GAMGAgglomeration& agglom,
  const dictionary& /*controlDict*/
)
:
  agglom_(agglom)
{}
mousse::autoPtr<mousse::GAMGProcAgglomeration> mousse::GAMGProcAgglomeration::New
(
  const word& type,
  GAMGAgglomeration& agglom,
  const dictionary& controlDict
)
{
  if (debug)
  {
    Info<< "GAMGProcAgglomeration::New(const word&, GAMGAgglomeration&"
       ", const dictionary&) : "
       "constructing GAMGProcAgglomeration"
      << endl;
  }
  GAMGAgglomerationConstructorTable::iterator cstrIter =
    GAMGAgglomerationConstructorTablePtr_->find(type);
  if (cstrIter == GAMGAgglomerationConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "GAMGProcAgglomeration::New(const word&, GAMGAgglomeration&"
      ", const dictionary&) "
    )   << "Unknown GAMGProcAgglomeration type "
      << type << " for GAMGAgglomeration " << agglom.type() << nl << nl
      << "Valid GAMGProcAgglomeration types are :" << endl
      << GAMGAgglomerationConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<GAMGProcAgglomeration>(cstrIter()(agglom, controlDict));
}

// Destructor
mousse::GAMGProcAgglomeration::~GAMGProcAgglomeration()
{}
