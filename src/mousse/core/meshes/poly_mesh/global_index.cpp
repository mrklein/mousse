// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "global_index.hpp"


// Constructors 
mousse::globalIndex::globalIndex
(
  const label localSize,
  const int tag,
  const label comm,
  const bool parallel
)
:
  offsets_{Pstream::nProcs(comm) + 1}
{
  labelList localSizes{Pstream::nProcs(comm), 0};
  localSizes[Pstream::myProcNo(comm)] = localSize;
  if (parallel) {
    Pstream::gatherList(localSizes, tag, comm);
    Pstream::scatterList(localSizes, tag, comm);
  }
  label offset = 0;
  offsets_[0] = 0;
  for (label procI = 0; procI < Pstream::nProcs(comm); procI++) {
    label oldOffset = offset;
    offset += localSizes[procI];
    if (offset < oldOffset) {
      FATAL_ERROR_IN
      (
        "globalIndex::globalIndex"
        "(const label, const int, const label, const bool)"
      )
      << "Overflow : sum of sizes " << localSizes
      << " exceeds capability of label (" << labelMax
      << "). Please recompile with larger datatype for label."
      << exit(FatalError);
    }
    offsets_[procI+1] = offset;
  }
}


mousse::globalIndex::globalIndex(const label localSize)
:
  offsets_{Pstream::nProcs() + 1}
{
  labelList localSizes{Pstream::nProcs(), 0};
  localSizes[Pstream::myProcNo()] = localSize;
  Pstream::gatherList(localSizes, Pstream::msgType());
  Pstream::scatterList(localSizes, Pstream::msgType());
  label offset = 0;
  offsets_[0] = 0;
  for (label procI = 0; procI < Pstream::nProcs(); procI++) {
    label oldOffset = offset;
    offset += localSizes[procI];
    if (offset < oldOffset) {
      FATAL_ERROR_IN("globalIndex::globalIndex(const label)")
        << "Overflow : sum of sizes " << localSizes
        << " exceeds capability of label (" << labelMax
        << "). Please recompile with larger datatype for label."
        << exit(FatalError);
    }
    offsets_[procI+1] = offset;
  }
}


mousse::globalIndex::globalIndex(const labelList& offsets)
:
  offsets_{offsets}
{}


mousse::globalIndex::globalIndex(Istream& is)
{
  is >> offsets_;
}


// Friend Operators 
mousse::Istream& mousse::operator>>(Istream& is, globalIndex& gi)
{
  return is >> gi.offsets_;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const globalIndex& gi)
{
  return os << gi.offsets_;
}

