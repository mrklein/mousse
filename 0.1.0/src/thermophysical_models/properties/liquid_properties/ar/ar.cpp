// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ar.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(Ar, 0);
  addToRunTimeSelectionTable(liquidProperties, Ar,);
  addToRunTimeSelectionTable(liquidProperties, Ar, Istream);
  addToRunTimeSelectionTable(liquidProperties, Ar, dictionary);
}
// Constructors 
mousse::Ar::Ar()
:
  liquidProperties
  (
    39.948,
    150.86,
    4.8981e+6,
    0.07459,
    0.291,
    83.78,
    6.88e+4,
    87.28,
    0.0,
    0.0,
    1.4138e+4
  ),
  rho_(151.922244, 0.286, 150.86, 0.2984),
  pv_(39.233, -1051.7, -3.5895, 5.0444e-05, 2),
  hl_(150.86, 218509.061780314, 0.352, 0.0, 0.0, 0.0),
  Cp_(4562.43116050866, -70.7770101131471, 0.367477721037349, 0.0, 0.0, 0.0),
  h_
  (
   -1460974.49982473,
    4562.43116050866,
   -35.3885050565735,
    0.122492573679116,
    0.0,
    0.0
  ),
  Cpg_(520.326424351657, 0.0, 0.0, 0.0, 0.0, 0.0),
  B_
  (
    0.000952488234705117,
   -0.379993992189847,
   -2022.62941824372,
    4633523580654.85,
    -302893761890458.0
  ),
  mu_(-8.868, 204.3, -0.3831, -1.3e-22, 10.0),
  mug_(8.386e-07, 0.6175, 75.377, -432.5),
  K_(0.1819, -0.0003176, -4.11e-06, 0.0, 0.0, 0.0),
  Kg_(0.0001236, 0.8262, -132.8, 16000),
  sigma_(150.86, 0.03823, 1.2927, 0.0, 0.0, 0.0),
  D_(147.18, 20.1, 39.948, 28) // note: Same as nHeptane
{}
mousse::Ar::Ar
(
  const liquidProperties& l,
  const NSRDSfunc5& density,
  const NSRDSfunc1& vapourPressure,
  const NSRDSfunc6& heatOfVapourisation,
  const NSRDSfunc0& heatCapacity,
  const NSRDSfunc0& enthalpy,
  const NSRDSfunc0& idealGasHeatCapacity,
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
mousse::Ar::Ar(Istream& is)
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
mousse::Ar::Ar(const dictionary& dict)
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
mousse::Ar::Ar(const Ar& liq)
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
