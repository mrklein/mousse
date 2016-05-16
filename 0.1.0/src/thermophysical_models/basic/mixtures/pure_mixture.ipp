// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pure_mixture.hpp"
#include "fv_mesh.hpp"


// Constructors 
template<class ThermoType>
mousse::pureMixture<ThermoType>::pureMixture
(
  const dictionary& thermoDict,
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicMixture{thermoDict, mesh, phaseName},
  mixture_{thermoDict.subDict("mixture")}
{}


// Member Functions 
template<class ThermoType>
void mousse::pureMixture<ThermoType>::read(const dictionary& thermoDict)
{
  mixture_ = ThermoType(thermoDict.subDict("mixture"));
}

