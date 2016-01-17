// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "partial_face_area_weight_ami.hpp"

// Private Member Functions 
template<class SourcePatch, class TargetPatch>
void mousse::partialFaceAreaWeightAMI<SourcePatch, TargetPatch>::setNextFaces
(
  label& startSeedI,
  label& srcFaceI,
  label& tgtFaceI,
  const boolList& mapFlag,
  labelList& seedFaces,
  const DynamicList<label>& visitedFaces,
  const bool /*errorOnNotFound*/
) const
{
  faceAreaWeightAMI<SourcePatch, TargetPatch>::setNextFaces
  (
    startSeedI,
    srcFaceI,
    tgtFaceI,
    mapFlag,
    seedFaces,
    visitedFaces,
    false // no error on not found
  );
}


// Constructors 
template<class SourcePatch, class TargetPatch>
mousse::partialFaceAreaWeightAMI<SourcePatch, TargetPatch>::
partialFaceAreaWeightAMI
(
  const SourcePatch& srcPatch,
  const TargetPatch& tgtPatch,
  const scalarField& srcMagSf,
  const scalarField& tgtMagSf,
  const faceAreaIntersect::triangulationMode& triMode,
  const bool reverseTarget,
  const bool requireMatch
)
:
  faceAreaWeightAMI<SourcePatch, TargetPatch>
  {
    srcPatch,
    tgtPatch,
    srcMagSf,
    tgtMagSf,
    triMode,
    reverseTarget,
    requireMatch
  }
{}


// Destructor
template<class SourcePatch, class TargetPatch>
mousse::partialFaceAreaWeightAMI<SourcePatch, TargetPatch>::
~partialFaceAreaWeightAMI()
{}


// Member Functions 
template<class SourcePatch, class TargetPatch>
bool mousse::partialFaceAreaWeightAMI<SourcePatch, TargetPatch>::conformal() const
{
  return false;
}


template<class SourcePatch, class TargetPatch>
void mousse::partialFaceAreaWeightAMI<SourcePatch, TargetPatch>::calculate
(
  labelListList& srcAddress,
  scalarListList& srcWeights,
  labelListList& tgtAddress,
  scalarListList& tgtWeights,
  label srcFaceI,
  label tgtFaceI
)
{
  bool ok =
    this->initialise
    (
      srcAddress,
      srcWeights,
      tgtAddress,
      tgtWeights,
      srcFaceI,
      tgtFaceI
    );
  if (!ok)
  {
    return;
  }
  // temporary storage for addressing and weights
  List<DynamicList<label>> srcAddr(this->srcPatch_.size());
  List<DynamicList<scalar>> srcWght(srcAddr.size());
  List<DynamicList<label>> tgtAddr(this->tgtPatch_.size());
  List<DynamicList<scalar>> tgtWght(tgtAddr.size());
  faceAreaWeightAMI<SourcePatch, TargetPatch>::calcAddressing
  (
    srcAddr,
    srcWght,
    tgtAddr,
    tgtWght,
    srcFaceI,
    tgtFaceI
  );
  // transfer data to persistent storage
  FOR_ALL(srcAddr, i)
  {
    srcAddress[i].transfer(srcAddr[i]);
    srcWeights[i].transfer(srcWght[i]);
  }
  FOR_ALL(tgtAddr, i)
  {
    tgtAddress[i].transfer(tgtAddr[i]);
    tgtWeights[i].transfer(tgtWght[i]);
  }
}
