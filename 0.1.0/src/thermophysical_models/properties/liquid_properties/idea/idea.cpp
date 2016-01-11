// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "idea.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(IDEA, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, IDEA,);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, IDEA, Istream);
  ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, IDEA, dictionary);
}
// Constructors 
mousse::IDEA::IDEA()
:
  liquidProperties
  (
    142.26,
    618.074,
    2.11e+6,
    0.523,
    0.247,
    242.67,
    3.4929e-2,
    447.3,
    1.7012e-30,
    0.3478,
    1.57e+4
  ),
  rho_(152.012105, 3.87150382e-1, 618.073893, 4.00790044e-1),
  pv_
  (
    8.4817774623e+01,
   -8.6782398353e+03,
   -9.1277694857,
    4.6153144498e-06,
    2.0
  ),
  hl_
  (
    618.074,
    2.1671983789e+05,
   -4.2413153435e+00,
    1.1656811532e+01,
   -1.1656446689e+01,
    4.3667661492
  ),
  Cp_(1.6604957e+3, -6.250871e-1, 6.1778552e-3, 0.0, 0.0, 0.0),
  h_(0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
  Cpg_
  (
    1.0457515243e+03,
    3.4410492875e+03,
    1.5976862298e+03,
    2.4697705752e+03,
    7.3699710536e+02
  ),
  B_
  (
    0.00337351091119935,
   -4.13606494008504,
   -534560.916470464,
   -1.13364022911762e+19,
    2.80704220402713e+21
  ),
  mu_(-6.9645853822e+01, 4.4390635942e+03, 8.4680722718e+00, 0.0, 0.0),
  mug_(4.2629382158e-08, 8.8144402122e-01, 9.6918097636e+01, 0.0),
  K_(2.03684e-01, -2.3168e-04, 0.0, 0.0, 0.0, 0.0),
  Kg_
  (
   -5.664925956707e+02,
    8.896721676320e-01,
   -2.849783998688e+09,
    6.914935658053e+05
  ),
  sigma_
  (
    618.074,
    8.3846525429e-03,
   -1.0044759047e+01,
    2.7261918781e+01,
   -2.5529134309e+01,
    8.6488806234
  ),
  D_(147.18, 20.1, 142.2, 28.0) // note: Same as nHeptane
{}
mousse::IDEA::IDEA
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
mousse::IDEA::IDEA(Istream& is)
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
mousse::IDEA::IDEA(const dictionary& dict)
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
mousse::IDEA::IDEA(const IDEA& liq)
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
