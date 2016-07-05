// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "standard_phase_change.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "thermo_single_layer.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(standardPhaseChange, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  phaseChangeModel,
  standardPhaseChange,
  dictionary
);


// Protected Member Functions 
scalar standardPhaseChange::Sh
(
  const scalar Re,
  const scalar Sc
) const
{
  if (Re < 5.0E+05) {
    return 0.664*sqrt(Re)*cbrt(Sc);
  } else {
    return 0.037*pow(Re, 0.8)*cbrt(Sc);
  }
}


// Constructors 
standardPhaseChange::standardPhaseChange
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  phaseChangeModel{typeName, owner, dict},
  deltaMin_{readScalar(coeffDict_.lookup("deltaMin"))},
  L_{readScalar(coeffDict_.lookup("L"))},
  TbFactor_{coeffDict_.lookupOrDefault<scalar>("TbFactor", 1.1)}
{}


// Destructor 
standardPhaseChange::~standardPhaseChange()
{}


// Member Functions 
void standardPhaseChange::correctModel
(
  const scalar dt,
  scalarField& availableMass,
  scalarField& dMass,
  scalarField& dEnergy
)
{
  const thermoSingleLayer& film = filmType<thermoSingleLayer>();
  // set local thermo properties
  const SLGThermo& thermo = film.thermo();
  const filmThermoModel& filmThermo = film.filmThermo();
  const label vapId = thermo.carrierId(filmThermo.name());
  // retrieve fields from film model
  const scalarField& delta = film.delta();
  const scalarField& YInf = film.YPrimary()[vapId];
  const scalarField& pInf = film.pPrimary();
  const scalarField& T = film.T();
  const scalarField& rho = film.rho();
  const scalarField& rhoInf = film.rhoPrimary();
  const scalarField& muInf = film.muPrimary();
  const scalarField& magSf = film.magSf();
  const vectorField dU{film.UPrimary() - film.Us()};
  const scalarField limMass
  {
    max(scalar(0.0), availableMass - deltaMin_*rho*magSf)
  };
  FOR_ALL(dMass, cellI) {
    if (delta[cellI] <= deltaMin_)
      continue;
    // cell pressure [Pa]
    const scalar pc = pInf[cellI];
    // calculate the boiling temperature
    const scalar Tb = filmThermo.Tb(pc);
    // local temperature - impose lower limit of 200 K for stability
    const scalar Tloc = min(TbFactor_*Tb, max(200.0, T[cellI]));
    // saturation pressure [Pa]
    const scalar pSat = filmThermo.pv(pc, Tloc);
    // latent heat [J/kg]
    const scalar hVap = filmThermo.hl(pc, Tloc);
    // calculate mass transfer
    if (pSat >= 0.95*pc) {
      // boiling
      const scalar Cp = filmThermo.Cp(pc, Tloc);
      const scalar Tcorr = max(0.0, T[cellI] - Tb);
      const scalar qCorr = limMass[cellI]*Cp*(Tcorr);
      dMass[cellI] = qCorr/hVap;
    } else {
      // Primary region density [kg/m3]
      const scalar rhoInfc = rhoInf[cellI];
      // Primary region viscosity [Pa.s]
      const scalar muInfc = muInf[cellI];
      // Reynolds number
      const scalar Re = rhoInfc*mag(dU[cellI])*L_/muInfc;
      // molecular weight of vapour [kg/kmol]
      const scalar Wvap = thermo.carrier().W(vapId);
      // molecular weight of liquid [kg/kmol]
      const scalar Wliq = filmThermo.W();
      // vapour mass fraction at interface
      const scalar Ys = Wliq*pSat/(Wliq*pSat + Wvap*(pc - pSat));
      // vapour diffusivity [m2/s]
      const scalar Dab = filmThermo.D(pc, Tloc);
      // Schmidt number
      const scalar Sc = muInfc/(rhoInfc*(Dab + ROOTVSMALL));
      // Sherwood number
      const scalar Sh = this->Sh(Re, Sc);
      // mass transfer coefficient [m/s]
      const scalar hm = Sh*Dab/(L_ + ROOTVSMALL);
      // add mass contribution to source
      dMass[cellI] =
        dt*magSf[cellI]*rhoInfc*hm*(Ys - YInf[cellI])/(1.0 - Ys);
    }
    dMass[cellI] = min(limMass[cellI], max(0.0, dMass[cellI]));
    dEnergy[cellI] = dMass[cellI]*hVap;
  }
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
