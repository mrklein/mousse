// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "c2_h6.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(C2H6, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, C2H6,);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, C2H6, Istream);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, C2H6, dictionary);
}
// Constructors 
mousse::C2H6::C2H6()
:
  liquidProperties
  (
    30.070,
    305.32,
    4.872e+6,
    0.14550,
    0.279,
    90.35,
    1.13,
    184.55,
    0.0,
    0.0995,
    1.24e+4
  ),
  rho_(57.499854, 0.27937, 305.32, 0.29187),
  pv_(51.857, -2598.7, -5.1283, 1.4913e-05, 2.0),
  hl_(305.32, 701396.740937812, 0.60646, -0.55492, 0.32799, 0.0),
  Cp_
  (
    305.32,
    8.02554965861611,
    2983.63817758563,
    167.548325566287,
   -343.93389207094
  ),
  h_(0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
  Cpg_(1341.07083471899, 4463.58496840705, 1655.5, 2435.08480212837, 752.87),
  B_
  (
    0.00269205187894912,
   -2.05221150648487,
   -47721.9820419022,
    2.24808779514466e+15,
   -3.23910874625873e+17
  ),
  mu_(-3.4134, 197.05, -1.2193, -9.2023e-26, 10.0),
  mug_(2.5906e-07, 0.67988, 98.902, 0.0),
  K_(0.35758, -0.0011458, 6.1866e-07, 0.0, 0.0, 0.0),
  Kg_(7.3869e-05, 1.1689, 500.73, 0.0),
  sigma_(305.32, 0.048643, 1.1981, 0.0, 0.0, 0.0),
  D_(147.18, 20.1, 30.070, 28) // note: Same as nHeptane
{}
mousse::C2H6::C2H6
(
  const liquidProperties& l,
  const NSRDSfunc5& density,
  const NSRDSfunc1& vapourPressure,
  const NSRDSfunc6& heatOfVapourisation,
  const NSRDSfunc14& heatCapacity,
  const NSRDSfunc0& enthalpy,
  const NSRDSfunc7& idealGasHeatCapacity,
  const NSRDSfunc4& secondVirialCoeff,
  const NSRDSfunc1& dynamicViscosity,
  const NSRDSfunc2& vapourDynamicViscosity,
  const NSRDSfunc0& thermalConductivity,
  const NSRDSfunc2& vapourThermalConductivity,
  const NSRDSfunc6& surfaceTension,
  const APIdiffCoefFunc& vapourDiffussivity
)
:
  liquidProperties(l),
  rho_(density),
  pv_(vapourPressure),
  hl_(heatOfVapourisation),
  Cp_(heatCapacity),
  h_(enthalpy),
  Cpg_(idealGasHeatCapacity),
  B_(secondVirialCoeff),
  mu_(dynamicViscosity),
  mug_(vapourDynamicViscosity),
  K_(thermalConductivity),
  Kg_(vapourThermalConductivity),
  sigma_(surfaceTension),
  D_(vapourDiffussivity)
{}
mousse::C2H6::C2H6(Istream& is)
:
  liquidProperties(is),
  rho_(is),
  pv_(is),
  hl_(is),
  Cp_(is),
  h_(is),
  Cpg_(is),
  B_(is),
  mu_(is),
  mug_(is),
  K_(is),
  Kg_(is),
  sigma_(is),
  D_(is)
{}
mousse::C2H6::C2H6(const dictionary& dict)
:
  liquidProperties(dict),
  rho_(dict.subDict("rho")),
  pv_(dict.subDict("pv")),
  hl_(dict.subDict("hl")),
  Cp_(dict.subDict("Cp")),
  h_(dict.subDict("h")),
  Cpg_(dict.subDict("Cpg")),
  B_(dict.subDict("B")),
  mu_(dict.subDict("mu")),
  mug_(dict.subDict("mug")),
  K_(dict.subDict("K")),
  Kg_(dict.subDict("Kg")),
  sigma_(dict.subDict("sigma")),
  D_(dict.subDict("D"))
{}
mousse::C2H6::C2H6(const C2H6& liq)
:
  liquidProperties(liq),
  rho_(liq.rho_),
  pv_(liq.pv_),
  hl_(liq.hl_),
  Cp_(liq.Cp_),
  h_(liq.h_),
  Cpg_(liq.Cpg_),
  B_(liq.B_),
  mu_(liq.mu_),
  mug_(liq.mug_),
  K_(liq.K_),
  Kg_(liq.Kg_),
  sigma_(liq.sigma_),
  D_(liq.D_)
{}
