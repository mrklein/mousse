// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_baffle.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
namespace mousse
{
namespace regionModels
{
namespace thermalBaffleModels
{
inline tmp<scalarField> thermalBaffle::he
(
  const scalarField& p,
  const scalarField& T,
  const label patchI
) const
{
  return thermo_->he(p, T, patchI);
}
inline tmp<volScalarField> thermalBaffle::he() const
{
  return thermo_->he();
}
}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse
