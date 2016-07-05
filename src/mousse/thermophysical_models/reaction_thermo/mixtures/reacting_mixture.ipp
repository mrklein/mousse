// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_mixture.hpp"
#include "fv_mesh.hpp"


// Constructors 
template<class ThermoType>
mousse::reactingMixture<ThermoType>::reactingMixture
(
  const dictionary& thermoDict,
  const fvMesh& mesh,
  const word& phaseName
)
:
  speciesTable{},
  autoPtr<chemistryReader<ThermoType>>
  {
    chemistryReader<ThermoType>::New(thermoDict, *this)
  },
  multiComponentMixture<ThermoType>
  {
    thermoDict,
    *this,
    autoPtr<chemistryReader<ThermoType>>::operator()().speciesThermo(),
    mesh,
    phaseName
  },
  PtrList<Reaction<ThermoType>>
  {
    autoPtr<chemistryReader<ThermoType>>::operator()().reactions()
  }
{
  autoPtr<chemistryReader<ThermoType>>::clear();
}


// Member Functions 
template<class ThermoType>
void mousse::reactingMixture<ThermoType>::read(const dictionary& /*thermoDict*/)
{}

