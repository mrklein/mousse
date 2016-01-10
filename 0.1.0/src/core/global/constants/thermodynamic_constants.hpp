// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef thermodynamic_constants_hpp_
#define thermodynamic_constants_hpp_
namespace mousse
{
namespace constant
{
namespace thermodynamic
{
  //- Universal gas constant (default in [J/(kmol K)])
  extern const scalar RR;
  //- Standard pressure (default in [Pa])
  extern const scalar Pstd;
  //- Standard temperature (default in [K])
  extern const scalar Tstd;
}  // namespace thermodynamic
}  // namespace constant
}  // namespace mousse
#endif
