// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "b_c10_h7_ch3.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(bC10H7CH3, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, bC10H7CH3,);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, bC10H7CH3, Istream);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, bC10H7CH3, dictionary);

}


// Constructors 
mousse::bC10H7CH3::bC10H7CH3()
:
  liquidProperties
  {
    142.2,
    761.0,
    3.25e+6,
    0.507,
    0.260,
    307.73,
    1.7374e+1,
    514.20,
    1.4010e-30,
    0.3459,
    1.987e+4
  },
  rho_{67.36014, 0.23843, 761, 0.2559},
  pv_{134.31, -12103, -16.195, 6.9659e-06, 2},
  hl_{761.0, 513150.492264416, 0.4044, 0.0, 0.0, 0.0},
  Cp_{811.322081575246, 2.30225035161744, 0.0008628691983122, 0.0, 0.0, 0.0},
  h_
  {
    45001.2311880177,
    811.322081575246,
    1.15112517580872,
    0.000287623066104079,
    0.0,
    0.0
  },
  Cpg_{760.126582278481, 2699.08579465542, 1564.1, 1994.51476793249, 727.49},
  B_
  {
    0.00229430379746835,
   -3.53720112517581,
   -1067158.93108298,
    2.29746835443038e+18,
   -2.68438818565401e+21
  },
  mu_{-63.276, 4219, 7.5549, 0.0, 0.0},
  mug_{2.1791e-06, 0.3717, 712.53, 0.0},
  K_{0.1962, -0.00018414, 0.0, 0.0, 0.0, 0.0},
  Kg_{0.4477, -0.1282, -345.89, 2340100},
  sigma_{761.0, 0.066442, 1.2634, 0.0, 0.0, 0.0},
  D_{147.18, 20.1, 142.2, 28} // note: Same as nHeptane
{}


mousse::bC10H7CH3::bC10H7CH3
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
  liquidProperties{l},
  rho_{density},
  pv_{vapourPressure},
  hl_{heatOfVapourisation},
  Cp_{heatCapacity},
  h_{enthalpy},
  Cpg_{idealGasHeatCapacity},
  B_{secondVirialCoeff},
  mu_{dynamicViscosity},
  mug_{vapourDynamicViscosity},
  K_{thermalConductivity},
  Kg_{vapourThermalConductivity},
  sigma_{surfaceTension},
  D_{vapourDiffussivity}
{}


mousse::bC10H7CH3::bC10H7CH3(Istream& is)
:
  liquidProperties{is},
  rho_{is},
  pv_{is},
  hl_{is},
  Cp_{is},
  h_{is},
  Cpg_{is},
  B_{is},
  mu_{is},
  mug_{is},
  K_{is},
  Kg_{is},
  sigma_{is},
  D_{is}
{}


mousse::bC10H7CH3::bC10H7CH3(const dictionary& dict)
:
  liquidProperties{dict},
  rho_{dict.subDict("rho")},
  pv_{dict.subDict("pv")},
  hl_{dict.subDict("hl")},
  Cp_{dict.subDict("Cp")},
  h_{dict.subDict("h")},
  Cpg_{dict.subDict("Cpg")},
  B_{dict.subDict("B")},
  mu_{dict.subDict("mu")},
  mug_{dict.subDict("mug")},
  K_{dict.subDict("K")},
  Kg_{dict.subDict("Kg")},
  sigma_{dict.subDict("sigma")},
  D_{dict.subDict("D")}
{}


mousse::bC10H7CH3::bC10H7CH3(const bC10H7CH3& liq)
:
  liquidProperties{liq},
  rho_{liq.rho_},
  pv_{liq.pv_},
  hl_{liq.hl_},
  Cp_{liq.Cp_},
  h_{liq.h_},
  Cpg_{liq.Cpg_},
  B_{liq.B_},
  mu_{liq.mu_},
  mug_{liq.mug_},
  K_{liq.K_},
  Kg_{liq.Kg_},
  sigma_{liq.sigma_},
  D_{liq.D_}
{}

