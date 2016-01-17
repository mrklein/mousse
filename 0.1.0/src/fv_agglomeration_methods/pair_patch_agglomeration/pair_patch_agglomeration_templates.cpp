// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair_patch_agglomeration.hpp"
template<class Type>
void mousse::pairPatchAgglomeration::restrictField
(
  Field<Type>& cf,
  const Field<Type>& ff,
  const label fineLevelIndex
) const
{
  const labelList& fineToCoarse = restrictAddressing_[fineLevelIndex];
  if (ff.size() != fineToCoarse.size())
  {
    FATAL_ERROR_IN
    (
      "void pairPatchAgglomeration::restrictField"
      "(Field<Type>& cf, const Field<Type>& ff, "
      "const label fineLevelIndex) const"
    )   << "field does not correspond to level " << fineLevelIndex
      << " sizes: field = " << ff.size()
      << " level = " << fineToCoarse.size()
      << abort(FatalError);
  }
  cf = pTraits<Type>::zero;
  FOR_ALL(ff, i)
  {
    cf[fineToCoarse[i]] += ff[i];
  }
}
template<class Type>
void mousse::pairPatchAgglomeration::prolongField
(
  Field<Type>& ff,
  const Field<Type>& cf,
  const label coarseLevelIndex
) const
{
  const labelList& fineToCoarse = restrictAddressing_[coarseLevelIndex];
  FOR_ALL(fineToCoarse, i)
  {
    ff[i] = cf[fineToCoarse[i]];
  }
}
