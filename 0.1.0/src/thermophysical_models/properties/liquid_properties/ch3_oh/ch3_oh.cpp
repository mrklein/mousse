// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ch3_oh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(CH3OH, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, CH3OH,);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, CH3OH, Istream);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, CH3OH, dictionary);
}
// Constructors 
mousse::CH3OH::CH3OH()
:
  liquidProperties
  (
    32.042,
    512.58,
    8.0959e+6,
    0.1178,
    0.224,
    175.47,
    1.054e-1,
    337.85,
    5.6706e-30,
    0.5656,
    2.9523e+4
  ),
  rho_(73.952936, 0.27192, 512.58, 0.2331),
  pv_(109.93, -7471.3, -13.988, 0.015281, 1.0),
  hl_(512.58, 1644716.30984333, 0.3766, 0.0, 0.0, 0.0),
  Cp_
  (
    3358.09250358904,
   -11.8781599151114,
    0.0305536483365583,
    0.0,
    0.0,
    0.0
  ),
  h_
  (
   -8190474.32066862,
    3358.09250358904,
   -5.93907995755571,
    0.0101845494455194,
    0.0,
    0.0
  ),
  Cpg_(1226.9521253355, 2772.92303851195, 1963, 1733.66206853505, 909.6),
  B_
  (
   -0.0199737844079645,
    19.3496036452157,
   -3342487.98452032,
    2.40808938268523e+19,
   -6.85787404032208e+21
  ),
  mu_(-7.288, 1065.3, -0.6657, 0.0, 0.0),
  mug_(3.0663e-07, 0.69655, 205.0, 0.0),
  K_(0.2837, -0.000281, 0.0, 0.0, 0.0, 0.0),
  Kg_(-7.763, 1.0279, -74360000.0, 6770000000.0),
  sigma_(512.58, 0.056, -0.00014583, 1.08e-07, 0.0, 0.0),
  D_(147.18, 20.1, 32.042, 28.0) // note: Same as nHeptane
{}
mousse::CH3OH::CH3OH
(
  const liquidProperties& l,
  const NSRDSfunc5& density,
  const NSRDSfunc1& vapourPressure,
  const NSRDSfunc6& heatOfVapourisation,
  const NSRDSfunc0& heatCapacity,
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
mousse::CH3OH::CH3OH(Istream& is)
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
mousse::CH3OH::CH3OH(const dictionary& dict)
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
mousse::CH3OH::CH3OH(const CH3OH& liq)
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
