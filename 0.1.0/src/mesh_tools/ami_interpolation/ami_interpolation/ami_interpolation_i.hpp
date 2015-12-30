// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class SourcePatch, class TargetPatch>
inline mousse::label
mousse::AMIInterpolation<SourcePatch, TargetPatch>::singlePatchProc() const
{
  return singlePatchProc_;
}
template<class SourcePatch, class TargetPatch>
inline mousse::scalar
mousse::AMIInterpolation<SourcePatch, TargetPatch>::lowWeightCorrection() const
{
  return lowWeightCorrection_;
}
template<class SourcePatch, class TargetPatch>
inline bool
mousse::AMIInterpolation<SourcePatch, TargetPatch>::
applyLowWeightCorrection() const
{
  return lowWeightCorrection_ > 0;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcMagSf() const
{
  return srcMagSf_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::labelListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcAddress() const
{
  return srcAddress_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::scalarListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcWeights() const
{
  return srcWeights_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcWeightsSum() const
{
  return srcWeightsSum_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::mapDistribute&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcMap() const
{
  return srcMapPtr_();
}
template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtMagSf() const
{
  return tgtMagSf_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::labelListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtAddress() const
{
  return tgtAddress_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::scalarListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtWeights() const
{
  return tgtWeights_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtWeightsSum() const
{
  return tgtWeightsSum_;
}
template<class SourcePatch, class TargetPatch>
inline const mousse::mapDistribute&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtMap() const
{
  return tgtMapPtr_();
}
