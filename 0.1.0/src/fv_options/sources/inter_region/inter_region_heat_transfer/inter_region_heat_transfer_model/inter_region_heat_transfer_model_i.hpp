// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::word&
mousse::fv::interRegionHeatTransferModel::nbrRegionName() const
{
  return nbrRegionName_;
}
inline const mousse::meshToMesh&
mousse::fv::interRegionHeatTransferModel::meshInterp() const
{
  if (!meshInterpPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "const meshToMesh& interRegionHeatTransferModel::meshInterp() const"
    )   << "Interpolation object not set"
      << abort(FatalError);
  }
  return meshInterpPtr_();
}
inline const mousse::volScalarField&
mousse::fv::interRegionHeatTransferModel::htc() const
{
  return  htc_;
}
inline const mousse::fv::interRegionHeatTransferModel&
mousse::fv::interRegionHeatTransferModel::nbrModel() const
{
  if (nbrModel_ == NULL)
  {
    FATAL_ERROR_IN("const interRegionHeatTransferModel& nbrModel() const")
      << "Neighbour model not set"
      << abort(FatalError);
  }
  return *nbrModel_;
}
inline mousse::fv::interRegionHeatTransferModel&
mousse::fv::interRegionHeatTransferModel::nbrModel()
{
  if (nbrModel_ == NULL)
  {
    FATAL_ERROR_IN("interRegionHeatTransferModel& nbrModel()")
      << "Neighbour model not set"
      << abort(FatalError);
  }
  return *nbrModel_;
}
