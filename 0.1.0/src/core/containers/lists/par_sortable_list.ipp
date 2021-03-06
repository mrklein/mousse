// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "par_sortable_list.hpp"
#include "sortable_list.hpp"
#include "pstream.hpp"
#include "list_list_ops.hpp"
#include "pstream_reduce_ops.hpp"


// Private Member Functions 
template<class Type>
void mousse::ParSortableList<Type>::write
(
  const List<Type>& elems,
  Ostream& os
) const
{
  os << '(';
  FOR_ALL(elems, elemI) {
    os << ' ' << elems[elemI];
  }
  os << ')';
}


// Copy src, starting at destI into dest.
template<class Type>
void mousse::ParSortableList<Type>::copyInto
(
  const List<Type>& values,
  const labelList& indices,
  const label fromProcNo,
  label& destI,
  List<taggedValue>& dest
) const
{
  FOR_ALL(values, elemI) {
    taggedValue& tagVal = dest[destI];
    tagVal.value() = values[elemI];
    tagVal.index() = indices[elemI];
    tagVal.procID() = fromProcNo;
    destI++;
  }
}


template<class Type>
void mousse::ParSortableList<Type>::getPivots
(
  const List<Type>& elems,
  List<Type>& pivots
) const
{
  pivots.setSize(Pstream::nProcs());
  label pivotPos = 0;
  FOR_ALL(pivots, pivotI) {
    pivots[pivotI] = elems[pivotPos];
    pivotPos += elems.size()/Pstream::nProcs();
  }
}


template<class Type>
void mousse::ParSortableList<Type>::checkAndSend
(
  List<Type>& values,
  labelList& indices,
  const label bufSize,
  const label destProcI
) const
{
  if (destProcI != Pstream::myProcNo()) {
    values.setSize(bufSize);
    indices.setSize(bufSize);
    if (debug) {
      Pout <<  "Sending to " << destProcI << " elements:" << values
        << endl;
    }

    {
      OPstream toSlave
      {
        Pstream::blocking,
        static_cast<int>(destProcI)
      };
      toSlave << values << indices;
    }

  }
}


// Constructors 
// Construct from List, sorting the elements
template<class Type>
mousse::ParSortableList<Type>::ParSortableList(const UList<Type>& values)
:
  List<Type>{values},
  indices_{0},
  procs_{0}
{
  sort();
}


// Construct given size. Sort later on.
template<class Type>
mousse::ParSortableList<Type>::ParSortableList(const label size)
:
  List<Type>{size},
  indices_{0},
  procs_{0}
{}


// Member Functions 
// Sort
template<class Type>
void mousse::ParSortableList<Type>::sort()
{
  //
  // 0. Get total size of dataset.
  //
  label n = this->size();
  reduce(n, sumOp<label>());
  // 1. Sort list locally
  SortableList<Type> sorted{*this};
  // Collect elements at pivot points
  labelListList sortedGatherList{Pstream::nProcs()};
  labelList& pivots = sortedGatherList[Pstream::myProcNo()];
  getPivots(sorted, pivots);
  if (debug) {
    Pout << "pivots:";
    write(pivots, Pout);
    Pout << endl;
  }
  //
  // 2. Combine pivotlist per processor onto master, sort, get pivots.
  //
  Pstream::gatherList(sortedGatherList);
  if (Pstream::master()) {
    labelList allPivots =
      ListListOps::combine<labelList>
      (
        sortedGatherList,
        accessOp<labelList>()
      );
    SortableList<Type> sortedPivots{allPivots};
    if (debug) {
      Pout << "allPivots:";
      write(allPivots, Pout);
      Pout << endl;
    }
    getPivots(sortedPivots, pivots);
  }
  Pstream::scatter(pivots);
  if (debug) {
    Pout << "new pivots:";
    write(pivots, Pout);
    Pout << endl;
  }
  //
  // 3. Distribute pivots & distribute.
  //
  label pivotI = 1;
  label destProcI = 0;
  // Buffer for my own data. Keep original index together with value.
  labelList ownValues{sorted.size()};
  labelList ownIndices{sorted.size()};
  label ownI = 0;
  // Buffer for sending data
  labelList sendValues{sorted.size()};
  labelList sendIndices{sorted.size()};
  label sendI = 0;
  FOR_ALL(sorted, sortedI) {
    if ((pivotI < Pstream::nProcs()) && (sorted[sortedI] > pivots[pivotI])) {
      checkAndSend(sendValues, sendIndices, sendI, destProcI);
      // Reset buffer.
      sendValues.setSize(sorted.size());
      sendIndices.setSize(sorted.size());
      sendI = 0;
      pivotI++;
      destProcI++;
    }
    if (destProcI != Pstream::myProcNo()) {
      sendValues[sendI] = sorted[sortedI];
      sendIndices[sendI] = sorted.indices()[sortedI];
      sendI++;
    } else {
      ownValues[ownI] = sorted[sortedI];
      ownIndices[ownI] = sorted.indices()[sortedI];
      ownI++;
    }
  }
  // Handle trailing send buffer
  if (sendI != 0) {
    checkAndSend(sendValues, sendIndices, sendI, destProcI);
  }
  // Print ownValues
  ownValues.setSize(ownI);
  ownIndices.setSize(ownI);
  if (debug & 2) {
    Pout << "Not sending (to myself) elements "
      << ownValues << endl;
  }
  //
  // 4. Combine pieces from all processors & sort. Use indices() from
  // SortableList to remember source processor number.
  //
  // Allocate receive buffer. Acc. to paper upper bound is 2*n/p
  // (n=total size, p=nProcs). Resize later on.
  List<taggedValue> combinedValues{2*n/Pstream::nProcs()};
  label combinedI = 0;
  for (label procI = 0; procI < Pstream::nProcs(); procI++) {
    if (procI == Pstream::myProcNo()) {
      if (debug & 2) {
        Pout << "Copying from own:" << ownValues << endl;
      }
      // Copy ownValues,ownIndices into combined buffer
      copyInto(ownValues, ownIndices, procI, combinedI, combinedValues);
    } else {
      labelList recValues;
      labelList recIndices;

      {
        if (debug) {
          Pout << "Receiving from " << procI << endl;
        }
        IPstream fromSlave{Pstream::blocking, static_cast<int>(procI)};
        fromSlave >> recValues >> recIndices;
        if (debug & 2) {
          Pout << "Received from " << procI
            << " elements:" << recValues << endl;
        }
      }
      if (debug) {
        Pout << "Copying starting at:" << combinedI << endl;
      }
      copyInto(recValues, recIndices, procI, combinedI, combinedValues);
    }
  }
  combinedValues.setSize(combinedI);
  // Sort according to values
  mousse::sort(combinedValues);
  // Copy into *this
  this->setSize(combinedI);
  indices_.setSize(combinedI);
  procs_.setSize(combinedI);
  FOR_ALL(combinedValues, elemI) {
    this->operator[](elemI) = combinedValues[elemI].value();
    indices_[elemI] = combinedValues[elemI].index();
    procs_[elemI] = combinedValues[elemI].procID();
  }
}
