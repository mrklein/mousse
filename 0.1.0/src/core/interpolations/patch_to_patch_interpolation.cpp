// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_to_patch_interpolation.hpp"
#include "demand_driven_data.hpp"
namespace mousse
{
// Static Data Members
template<class FromPatch, class ToPatch>
const scalar
PatchToPatchInterpolation<FromPatch, ToPatch>::directHitTol = 1e-5;
// Private Member Functions 
template<class FromPatch, class ToPatch>
const labelList&
PatchToPatchInterpolation<FromPatch, ToPatch>::pointAddr() const
{
  if (!pointAddressingPtr_)
  {
    calcPointAddressing();
  }
  return *pointAddressingPtr_;
}
template<class FromPatch, class ToPatch>
const FieldField<Field, scalar>&
PatchToPatchInterpolation<FromPatch, ToPatch>::pointWeights() const
{
  if (!pointWeightsPtr_)
  {
    calcPointAddressing();
  }
  return *pointWeightsPtr_;
}
template<class FromPatch, class ToPatch>
const labelList&
PatchToPatchInterpolation<FromPatch, ToPatch>::faceAddr() const
{
  if (!faceAddressingPtr_)
  {
    calcFaceAddressing();
  }
  return *faceAddressingPtr_;
}
template<class FromPatch, class ToPatch>
const FieldField<Field, scalar>&
PatchToPatchInterpolation<FromPatch, ToPatch>::faceWeights() const
{
  if (!faceWeightsPtr_)
  {
    calcFaceAddressing();
  }
  return *faceWeightsPtr_;
}
template<class FromPatch, class ToPatch>
void PatchToPatchInterpolation<FromPatch, ToPatch>::clearOut()
{
  deleteDemandDrivenData(pointAddressingPtr_);
  deleteDemandDrivenData(pointWeightsPtr_);
  deleteDemandDrivenData(pointDistancePtr_);
  deleteDemandDrivenData(faceAddressingPtr_);
  deleteDemandDrivenData(faceWeightsPtr_);
  deleteDemandDrivenData(faceDistancePtr_);
}
// Constructors 
// Construct from components
template<class FromPatch, class ToPatch>
PatchToPatchInterpolation<FromPatch, ToPatch>::PatchToPatchInterpolation
(
  const FromPatch& fromPatch,
  const ToPatch& toPatch,
  intersection::algorithm alg,
  const intersection::direction dir
)
:
  fromPatch_(fromPatch),
  toPatch_(toPatch),
  alg_(alg),
  dir_(dir),
  pointAddressingPtr_(NULL),
  pointWeightsPtr_(NULL),
  pointDistancePtr_(NULL),
  faceAddressingPtr_(NULL),
  faceWeightsPtr_(NULL),
  faceDistancePtr_(NULL)
{}
// Destructor
template<class FromPatch, class ToPatch>
PatchToPatchInterpolation<FromPatch, ToPatch>::~PatchToPatchInterpolation()
{
  clearOut();
}
// Member Functions 
template<class FromPatch, class ToPatch>
const scalarField&
PatchToPatchInterpolation<FromPatch, ToPatch>
::pointDistanceToIntersection() const
{
  if (!pointDistancePtr_)
  {
    calcPointAddressing();
  }
  return *pointDistancePtr_;
}
template<class FromPatch, class ToPatch>
const scalarField&
PatchToPatchInterpolation<FromPatch, ToPatch>
::faceDistanceToIntersection() const
{
  if (!faceDistancePtr_)
  {
    calcFaceAddressing();
  }
  return *faceDistancePtr_;
}
template<class FromPatch, class ToPatch>
bool PatchToPatchInterpolation<FromPatch, ToPatch>::movePoints()
{
  clearOut();
  return true;
}
}  // namespace mousse
#   include "calc_patch_to_patch_weights.cpp"
#   include "patch_to_patch_interpolate.cpp"
