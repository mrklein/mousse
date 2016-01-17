// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_agglomeration.hpp"
#include "map_distribute.hpp"
#include "global_index.hpp"

template<class Type>
void mousse::GAMGAgglomeration::gatherList
(
  const label comm,
  const labelList& procIDs,
  const Type& myVal,
  List<Type>& allVals,
  const int tag
)
{
  if (Pstream::myProcNo(comm) == procIDs[0])
  {
    allVals.setSize(procIDs.size());
    allVals[0] = myVal;
    for (label i = 1; i < procIDs.size(); i++)
    {
      IPstream fromSlave
      (
        Pstream::scheduled,
        procIDs[i],
        0,
        tag,
        comm
      );
      fromSlave >> allVals[i];
    }
  }
  else
  {
    OPstream toMaster
    (
      Pstream::scheduled,
      procIDs[0],
      0,
      tag,
      comm
    );
    toMaster << myVal;
  }
}


template<class Type>
void mousse::GAMGAgglomeration::restrictField
(
  Field<Type>& cf,
  const Field<Type>& ff,
  const labelList& fineToCoarse
) const
{
  cf = pTraits<Type>::zero;
  FOR_ALL(ff, i)
  {
    cf[fineToCoarse[i]] += ff[i];
  }
}


template<class Type>
void mousse::GAMGAgglomeration::restrictField
(
  Field<Type>& cf,
  const Field<Type>& ff,
  const label fineLevelIndex,
  const bool procAgglom
) const
{
  const labelList& fineToCoarse = restrictAddressing_[fineLevelIndex];
  if (!procAgglom && ff.size() != fineToCoarse.size())
  {
    FATAL_ERROR_IN
    (
      "void GAMGAgglomeration::restrictField"
      "(Field<Type>& cf, const Field<Type>& ff, "
      "const label fineLevelIndex) const"
    )
    << "field does not correspond to level " << fineLevelIndex
    << " sizes: field = " << ff.size()
    << " level = " << fineToCoarse.size()
    << abort(FatalError);
  }
  restrictField(cf, ff, fineToCoarse);
  label coarseLevelIndex = fineLevelIndex+1;
  if (procAgglom && hasProcMesh(coarseLevelIndex))
  {
    label fineComm = UPstream::parent(procCommunicator_[coarseLevelIndex]);
    const List<label>& procIDs = agglomProcIDs(coarseLevelIndex);
    const labelList& offsets = cellOffsets(coarseLevelIndex);
    globalIndex::gather
    (
      offsets,
      fineComm,
      procIDs,
      cf,
      UPstream::msgType(),
      Pstream::nonBlocking    //Pstream::scheduled
    );
  }
}


template<class Type>
void mousse::GAMGAgglomeration::restrictFaceField
(
  Field<Type>& cf,
  const Field<Type>& ff,
  const label fineLevelIndex
) const
{
  const labelList& fineToCoarse = faceRestrictAddressing_[fineLevelIndex];
  if (ff.size() != fineToCoarse.size())
  {
    FATAL_ERROR_IN
    (
      "void GAMGAgglomeration::restrictFaceField"
      "(Field<Type>& cf, const Field<Type>& ff, "
      "const label fineLevelIndex) const"
    )
    << "field does not correspond to level " << fineLevelIndex
    << " sizes: field = " << ff.size()
    << " level = " << fineToCoarse.size()
    << abort(FatalError);
  }
  cf = pTraits<Type>::zero;
  FOR_ALL(fineToCoarse, ffacei)
  {
    label cFace = fineToCoarse[ffacei];
    if (cFace >= 0)
    {
      cf[cFace] += ff[ffacei];
    }
  }
}


template<class Type>
void mousse::GAMGAgglomeration::prolongField
(
  Field<Type>& ff,
  const Field<Type>& cf,
  const label levelIndex,
  const bool procAgglom
) const
{
  const labelList& fineToCoarse = restrictAddressing_[levelIndex];
  label coarseLevelIndex = levelIndex+1;
  if (procAgglom && hasProcMesh(coarseLevelIndex))
  {
    label coarseComm = UPstream::parent
    (
      procCommunicator_[coarseLevelIndex]
    );
    const List<label>& procIDs = agglomProcIDs(coarseLevelIndex);
    const labelList& offsets = cellOffsets(coarseLevelIndex);
    label localSize = nCells_[levelIndex];
    Field<Type> allCf{localSize};
    globalIndex::scatter
    (
      offsets,
      coarseComm,
      procIDs,
      cf,
      allCf,
      UPstream::msgType(),
      Pstream::nonBlocking    //Pstream::scheduled
    );
    FOR_ALL(fineToCoarse, i)
    {
      ff[i] = allCf[fineToCoarse[i]];
    }
  }
  else
  {
    FOR_ALL(fineToCoarse, i)
    {
      ff[i] = cf[fineToCoarse[i]];
    }
  }
}
