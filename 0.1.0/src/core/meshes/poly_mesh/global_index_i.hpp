// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list_ops.hpp"
// Constructors 
mousse::globalIndex::globalIndex()
{}
mousse::globalIndex::globalIndex(const Xfer<labelList>& offsets)
:
  offsets_(offsets)
{}
// Member Functions 
inline mousse::labelList& mousse::globalIndex::offsets()
{
  return offsets_;
}
inline mousse::label mousse::globalIndex::offset(const label procI) const
{
  return offsets_[procI];
}
inline mousse::label mousse::globalIndex::localSize(const label procI) const
{
  return offsets_[procI+1] - offsets_[procI];
}
inline mousse::label mousse::globalIndex::localSize() const
{
  return localSize(Pstream::myProcNo());
}
inline mousse::label mousse::globalIndex::size() const
{
  return offsets_.last();
}
inline mousse::label mousse::globalIndex::toGlobal
(
  const label procI,
  const label i
) const
{
  return i + offsets_[procI];
}
inline mousse::label mousse::globalIndex::toGlobal(const label i) const
{
  return toGlobal(Pstream::myProcNo(), i);
}
//- Is on local processor
inline bool mousse::globalIndex::isLocal(const label procI, const label i) const
{
  return i >= offsets_[procI] && i < offsets_[procI+1];
}
inline bool mousse::globalIndex::isLocal(const label i) const
{
  return isLocal(Pstream::myProcNo(), i);
}
inline mousse::label mousse::globalIndex::toLocal(const label procI, const label i)
const
{
  label localI = i - offsets_[procI];
  if (localI < 0 || i >= offsets_[procI+1])
  {
    FATAL_ERROR_IN("globalIndex::toLocal(const label, const label)")
      << "Global " << i << " does not belong on processor "
      << procI << endl << "Offsets:" << offsets_
      << abort(FatalError);
  }
  return localI;
}
inline mousse::label mousse::globalIndex::toLocal(const label i) const
{
  return toLocal(Pstream::myProcNo(), i);
}
inline mousse::label mousse::globalIndex::whichProcID(const label i) const
{
  if (i < 0 || i >= size())
  {
    FATAL_ERROR_IN("globalIndex::whichProcID(const label)")
      << "Global " << i << " does not belong on any processor."
      << " Offsets:" << offsets_
      << abort(FatalError);
  }
  return findLower(offsets_, i+1);
}
