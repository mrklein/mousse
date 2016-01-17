// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "egr_mixture.hpp"
#include "fv_mesh.hpp"
// Static Data Members
template<class ThermoType>
const char* mousse::egrMixture<ThermoType>::specieNames_[3] = {"ft", "b", "egr"};
// Constructors 
template<class ThermoType>
mousse::egrMixture<ThermoType>::egrMixture
(
  const dictionary& thermoDict,
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicCombustionMixture
  (
    thermoDict,
    speciesTable(nSpecies_, specieNames_),
    mesh,
    phaseName
  ),
  stoicRatio_(thermoDict.lookup("stoichiometricAirFuelMassRatio")),
  fuel_(thermoDict.subDict("fuel")),
  oxidant_(thermoDict.subDict("oxidant")),
  products_(thermoDict.subDict("burntProducts")),
  mixture_("mixture", fuel_),
  ft_(Y("ft")),
  b_(Y("b")),
  egr_(Y("egr"))
{}
// Member Functions 
template<class ThermoType>
const ThermoType& mousse::egrMixture<ThermoType>::mixture
(
  const scalar ft,
  const scalar b,
  const scalar egr
) const
{
  if (ft < 0.0001)
  {
    return oxidant_;
  }
  else
  {
    scalar fu = b*ft + (1.0 - b)*fres(ft, stoicRatio().value());
    scalar ox = 1 - ft - (ft - fu)*stoicRatio().value();
    fu *= (1.0 - egr);
    ox *= (1.0 - egr);
    scalar pr = 1 - fu - ox;
    mixture_ = fu/fuel_.W()*fuel_;
    mixture_ += ox/oxidant_.W()*oxidant_;
    mixture_ += pr/products_.W()*products_;
    return mixture_;
  }
}
template<class ThermoType>
void mousse::egrMixture<ThermoType>::read(const dictionary& thermoDict)
{
  stoicRatio_ = thermoDict.lookup("stoichiometricAirFuelMassRatio");
  fuel_ = ThermoType(thermoDict.subDict("fuel"));
  oxidant_ = ThermoType(thermoDict.subDict("oxidant"));
  products_ = ThermoType(thermoDict.subDict("burntProducts"));
}
template<class ThermoType>
const ThermoType& mousse::egrMixture<ThermoType>::getLocalThermo
(
  const label speciei
) const
{
  if (speciei == 0)
  {
    return fuel_;
  }
  else if (speciei == 1)
  {
    return oxidant_;
  }
  else if (speciei == 2)
  {
    return products_;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "const ThermoType& mousse::egrMixture<ThermoType>::getLocalThermo"
      "("
        "const label "
      ") const"
    )   << "Unknown specie index " << speciei << ". Valid indices are 0..2"
      << abort(FatalError);
    return fuel_;
  }
}
