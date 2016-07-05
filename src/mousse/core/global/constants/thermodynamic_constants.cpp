// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermodynamic_constants.hpp"
#include "physico_chemical_constants.hpp"
namespace mousse
{
namespace constant
{
namespace thermodynamic
{
  // Note: the 1e3 converts from /mol to /kmol for consistency with the
  // SI choice of kg rather than g for mass.
  // This is not appropriate for USCS and will be changed to an entry in
  // the DimensionedConstants dictionary in etc/controlDict
  const scalar RR = 1e3*physicoChemical::R.value();
  const scalar Pstd = standard::Pstd.value();
  const scalar Tstd = standard::Tstd.value();
}  // namespace thermodynamic
}  // namespace constant
}  // namespace mousse
