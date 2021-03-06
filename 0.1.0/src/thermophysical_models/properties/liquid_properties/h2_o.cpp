// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "h2_o.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(H2O, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, H2O,);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, H2O, Istream);
ADD_TO_RUN_TIME_SELECTION_TABLE(liquidProperties, H2O, dictionary);

}


// Constructors 
mousse::H2O::H2O()
:
  liquidProperties
  {
    18.015,
    647.13,
    2.2055e+7,
    0.05595,
    0.229,
    273.16,
    6.113e+2,
    373.15,
    6.1709e-30,
    0.3449,
    4.7813e+4
  },
  rho_{98.343885, 0.30542, 647.13, 0.081},
  pv_{73.649, -7258.2, -7.3037, 4.1653e-06, 2},
  hl_{647.13, 2889425.47876769, 0.3199, -0.212, 0.25795, 0},
  Cp_
  {
    15341.1046350264,
   -116.019983347211,
    0.451013044684985,
   -0.000783569247849015,
    5.20127671384957e-07,
    0
  },
  h_
  {
   -17957283.7993676,
    15341.1046350264,
   -58.0099916736053,
    0.150337681561662,
   -0.000195892311962254,
    1.04025534276991e-07
  },
  Cpg_
  {
    1851.73466555648,
    1487.53816264224,
    2609.3,
    493.366638912018,
    1167.6
  },
  B_
  {
   -0.0012789342214821,
    1.4909797391063,
   -1563696.91923397,
    1.85445462114904e+19,
   -7.68082153760755e+21
  },
  mu_{-51.964, 3670.6, 5.7331, -5.3495e-29, 10},
  mug_{2.6986e-06, 0.498, 1257.7, -19570},
  K_{-0.4267, 0.0056903, -8.0065e-06, 1.815e-09, 0, 0},
  Kg_{6.977e-05, 1.1243, 844.9, -148850},
  sigma_{647.13, 0.18548, 2.717, -3.554, 2.047, 0},
  D_{15.0, 15.0, 18.015, 28}
{}


mousse::H2O::H2O
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


mousse::H2O::H2O(Istream& is)
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


mousse::H2O::H2O(const dictionary& dict)
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


mousse::H2O::H2O(const H2O& liq)
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

