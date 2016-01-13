// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_model.hpp"
inline const mousse::fvMesh&
mousse::regionModels::regionModel::primaryMesh() const
{
  return primaryMesh_;
}
inline const mousse::Time& mousse::regionModels::regionModel::time() const
{
  return time_;
}
inline const mousse::Switch& mousse::regionModels::regionModel::active() const
{
  return active_;
}
inline const mousse::Switch& mousse::regionModels::regionModel::infoOutput() const
{
  return infoOutput_;
}
inline const mousse::word& mousse::regionModels::regionModel::modelName() const
{
  return modelName_;
}
inline const mousse::fvMesh& mousse::regionModels::regionModel::regionMesh() const
{
  if (time_.foundObject<fvMesh>(regionName_))
  {
    return time_.lookupObject<fvMesh>(regionName_);
  }
  else if (!regionMeshPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline const mousse::fvMesh&"
      "mousse::regionModels::regionModel::regionMesh() const"
    )<< "Region mesh not available" << abort(FatalError);
  }
  return regionMeshPtr_();
}
inline mousse::fvMesh& mousse::regionModels::regionModel::regionMesh()
{
  if (time_.foundObject<fvMesh>(regionName_))
  {
    return const_cast<fvMesh&>
    (
      time_.lookupObject<fvMesh>(regionName_)
    );
  }
  else if (!regionMeshPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline mousse::fvMesh&"
      "mousse::regionModels::regionModel::regionMesh()"
    )<< "Region mesh not available" << abort(FatalError);
  }
  return regionMeshPtr_();
}
inline const mousse::dictionary& mousse::regionModels::regionModel::coeffs() const
{
  return coeffs_;
}
inline const mousse::dictionary&
mousse::regionModels::regionModel::solution() const
{
  return regionMesh().solutionDict();
}
inline const mousse::IOdictionary&
mousse::regionModels::regionModel::outputProperties() const
{
  if (!outputPropertiesPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline const mousse::IOdictionary& "
      "mousse::regionModels::regionModel::outputProperties() const"
    )
      << "outputProperties dictionary not available"
      << abort(FatalError);
  }
  return outputPropertiesPtr_();
}
inline mousse::IOdictionary&
mousse::regionModels::regionModel::outputProperties()
{
  if (!outputPropertiesPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline mousse::IOdictionary& "
      "mousse::regionModels::regionModel::outputProperties()"
    )
      << "outputProperties dictionary not available"
      << abort(FatalError);
  }
  return outputPropertiesPtr_();
}
inline bool mousse::regionModels::regionModel::isCoupledPatch
(
  const label regionPatchI
) const
{
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    if (intCoupledPatchIDs_[i] == regionPatchI)
    {
      return true;
    }
  }
  return false;
}
inline bool mousse::regionModels::regionModel::isRegionPatch
(
  const label primaryPatchI
) const
{
  FOR_ALL(primaryPatchIDs_, i)
  {
    if (primaryPatchIDs_[i] == primaryPatchI)
    {
      return true;
    }
  }
  return false;
}
inline const mousse::labelList&
mousse::regionModels::regionModel::primaryPatchIDs() const
{
  return primaryPatchIDs_;
}
inline const mousse::labelList&
mousse::regionModels::regionModel::intCoupledPatchIDs() const
{
  return intCoupledPatchIDs_;
}
inline mousse::label mousse::regionModels::regionModel::regionPatchID
(
  const label primaryPatchID
) const
{
  FOR_ALL(primaryPatchIDs_, i)
  {
    if (primaryPatchIDs_[i] == primaryPatchID)
    {
      return intCoupledPatchIDs_[i];
    }
  }
  return -1;
}
