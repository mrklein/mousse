// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multi_component_mixture.hpp"
// Private Member Functions 
template<class ThermoType>
const ThermoType& mousse::multiComponentMixture<ThermoType>::constructSpeciesData
(
  const dictionary& thermoDict
)
{
  FOR_ALL(species_, i)
  {
    speciesData_.set
    (
      i,
      new ThermoType(thermoDict.subDict(species_[i]))
    );
  }
  return speciesData_[0];
}
template<class ThermoType>
void mousse::multiComponentMixture<ThermoType>::correctMassFractions()
{
  // Multiplication by 1.0 changes Yt patches to "calculated"
  volScalarField Yt("Yt", 1.0*Y_[0]);
  for (label n=1; n<Y_.size(); n++)
  {
    Yt += Y_[n];
  }
  if (mag(max(Yt).value()) < ROOTVSMALL)
  {
    FATAL_ERROR_IN
    (
      "void mousse::multiComponentMixture<ThermoType>::"
      "correctMassFractions()"
    )
      << "Sum of mass fractions is zero for species " << this->species()
      << exit(FatalError);
  }
  FOR_ALL(Y_, n)
  {
    Y_[n] /= Yt;
  }
}
// Constructors 
template<class ThermoType>
mousse::multiComponentMixture<ThermoType>::multiComponentMixture
(
  const dictionary& thermoDict,
  const wordList& specieNames,
  const HashPtrTable<ThermoType>& thermoData,
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicSpecieMixture(thermoDict, specieNames, mesh, phaseName),
  speciesData_(species_.size()),
  mixture_("mixture", *thermoData[specieNames[0]]),
  mixtureVol_("volMixture", *thermoData[specieNames[0]])
{
  FOR_ALL(species_, i)
  {
    speciesData_.set
    (
      i,
      new ThermoType(*thermoData[species_[i]])
    );
  }
  correctMassFractions();
}
template<class ThermoType>
mousse::multiComponentMixture<ThermoType>::multiComponentMixture
(
  const dictionary& thermoDict,
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicSpecieMixture
  (
    thermoDict,
    thermoDict.lookup("species"),
    mesh,
    phaseName
  ),
  speciesData_(species_.size()),
  mixture_("mixture", constructSpeciesData(thermoDict)),
  mixtureVol_("volMixture", speciesData_[0])
{
  correctMassFractions();
}
// Member Functions 
template<class ThermoType>
const ThermoType& mousse::multiComponentMixture<ThermoType>::cellMixture
(
  const label celli
) const
{
  mixture_ = Y_[0][celli]/speciesData_[0].W()*speciesData_[0];
  for (label n=1; n<Y_.size(); n++)
  {
    mixture_ += Y_[n][celli]/speciesData_[n].W()*speciesData_[n];
  }
  return mixture_;
}
template<class ThermoType>
const ThermoType& mousse::multiComponentMixture<ThermoType>::patchFaceMixture
(
  const label patchi,
  const label facei
) const
{
  mixture_ =
    Y_[0].boundaryField()[patchi][facei]
   /speciesData_[0].W()*speciesData_[0];
  for (label n=1; n<Y_.size(); n++)
  {
    mixture_ +=
      Y_[n].boundaryField()[patchi][facei]
     /speciesData_[n].W()*speciesData_[n];
  }
  return mixture_;
}
template<class ThermoType>
const ThermoType& mousse::multiComponentMixture<ThermoType>::cellVolMixture
(
  const scalar p,
  const scalar T,
  const label celli
) const
{
  scalar rhoInv = 0.0;
  FOR_ALL(speciesData_, i)
  {
    rhoInv += Y_[i][celli]/speciesData_[i].rho(p, T);
  }
  mixtureVol_ =
    Y_[0][celli]/speciesData_[0].rho(p, T)/rhoInv*speciesData_[0];
  for (label n=1; n<Y_.size(); n++)
  {
    mixtureVol_ +=
      Y_[n][celli]/speciesData_[n].rho(p, T)/rhoInv*speciesData_[n];
  }
  return mixtureVol_;
}
template<class ThermoType>
const ThermoType& mousse::multiComponentMixture<ThermoType>::
patchFaceVolMixture
(
  const scalar p,
  const scalar T,
  const label patchi,
  const label facei
) const
{
  scalar rhoInv = 0.0;
  FOR_ALL(speciesData_, i)
  {
    rhoInv +=
      Y_[i].boundaryField()[patchi][facei]/speciesData_[i].rho(p, T);
  }
  mixtureVol_ =
    Y_[0].boundaryField()[patchi][facei]/speciesData_[0].rho(p, T)/rhoInv
   * speciesData_[0];
  for (label n=1; n<Y_.size(); n++)
  {
    mixtureVol_ +=
      Y_[n].boundaryField()[patchi][facei]/speciesData_[n].rho(p,T)
     / rhoInv*speciesData_[n];
  }
  return mixtureVol_;
}
template<class ThermoType>
void mousse::multiComponentMixture<ThermoType>::read
(
  const dictionary& thermoDict
)
{
  FOR_ALL(species_, i)
  {
    speciesData_[i] = ThermoType(thermoDict.subDict(species_[i]));
  }
}
