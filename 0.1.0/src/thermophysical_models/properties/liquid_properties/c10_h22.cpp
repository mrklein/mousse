// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "c10_h22.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(C10H22, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, C10H22,);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, C10H22, Istream);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, C10H22, dictionary);

}


// Constructors 
mousse::C10H22::C10H22()
:
  liquidProperties
  {
    142.285,
    617.70,
    2.11e+6,
    0.6,
    0.247,
    243.51,
    1.393,
    447.30,
    0.0,
    0.4923,
    1.57e+4
  },
  rho_{60.94208835, 0.25745, 617.7, 0.28912},
  pv_{112.73, -9749.6, -13.245, 7.1266e-06, 2.0},
  hl_{617.70, 464743.296904101, 0.39797, 0.0, 0.0, 0.0},
  Cp_
  {
    1958.18252099659,
   -1.39094071757388,
    0.00754612221948905,
    0.0,
    0.0,
    0.0
  },
  h_
  {
   -2699436.15229142,
    1958.18252099659,
   -0.695470358786942,
    0.00251537407316302,
    0.0,
    0.0
  },
  Cpg_{1175.10630073444, 3762.16748076045, 1614.1, 2658.04547211582, 742},
  B_
  {
    0.00337351091119935,
   -4.13606494008504,
   -534560.916470464,
   -1.13364022911762e+19,
    2.80704220402713e+21
  },
  mu_{-16.468, 1533.5, 0.7511, 0.0, 0.0},
  mug_{2.64e-08, 0.9487, 71.0, 0.0},
  K_{0.2063, -0.000254, 0.0, 0.0, 0.0, 0.0},
  Kg_{-668.4, 0.9323, -4071000000.0, 0.0},
  sigma_{617.70, 0.055435, 1.3095, 0.0, 0.0, 0.0},
  D_{147.18, 20.1, 142.285, 28.0} // note: Same as nHeptane
{}


mousse::C10H22::C10H22
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


mousse::C10H22::C10H22(Istream& is)
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


mousse::C10H22::C10H22(const dictionary& dict)
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


mousse::C10H22::C10H22(const C10H22& liq)
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

