// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::fvMesh& mousse::combustionModel::mesh() const
{
  return mesh_;
}
inline const mousse::surfaceScalarField& mousse::combustionModel::phi() const
{
  if (turbulencePtr_)
  {
    return turbulencePtr_->phi();
  }
  else
  {
    FATAL_ERROR_IN
    (
      "const mousse::compressibleTurbulenceModel& "
      "mousse::combustionModel::turbulence() const "
    )   << "turbulencePtr_ is empty. Please use "
      << "combustionModel::setTurbulence "
      << "(compressibleTurbulenceModel& )"
      << abort(FatalError);
    return turbulencePtr_->phi();
  }
}
inline const mousse::compressibleTurbulenceModel&
mousse::combustionModel::turbulence() const
{
  if (turbulencePtr_)
  {
    return *turbulencePtr_;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "const mousse::compressibleTurbulenceModel& "
      "mousse::combustionModel::turbulence() const "
    )   << "turbulencePtr_ is empty. Please use "
      << "combustionModel::setTurbulence "
      << "(compressibleTurbulenceModel& )"
      << abort(FatalError);
    return *turbulencePtr_;
  }
}
inline const mousse::Switch& mousse::combustionModel::active() const
{
  return active_;
}
inline void mousse::combustionModel::setTurbulence
(
  compressibleTurbulenceModel& turbModel
)
{
  turbulencePtr_ = &turbModel;
}
inline const mousse::dictionary& mousse::combustionModel::coeffs() const
{
  return coeffs_;
}
