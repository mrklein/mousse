// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class SourcePatch, class TargetPatch>
inline const mousse::labelList&
mousse::AMIMethod<SourcePatch, TargetPatch>::srcNonOverlap() const
{
  return srcNonOverlap_;
}
