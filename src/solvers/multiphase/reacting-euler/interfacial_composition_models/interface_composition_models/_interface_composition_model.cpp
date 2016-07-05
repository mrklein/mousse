// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_interface_composition_model.hpp"
#include "phase_model.hpp"
#include "phase_pair.hpp"
#include "pure_mixture.hpp"
#include "multi_component_mixture.hpp"
#include "rho_thermo.hpp"
// Private Member Functions 
template<class Thermo, class OtherThermo>
template<class ThermoType>
const typename mousse::multiComponentMixture<ThermoType>::thermoType&
mousse::InterfaceCompositionModel<Thermo, OtherThermo>::getLocalThermo
(
  const word& speciesName,
  const multiComponentMixture<ThermoType>& globalThermo
) const
{
  return
    globalThermo.getLocalThermo
    (
      globalThermo.species()
      [
        speciesName
      ]
    );
}
template<class Thermo, class OtherThermo>
template<class ThermoType>
const typename mousse::pureMixture<ThermoType>::thermoType&
mousse::InterfaceCompositionModel<Thermo, OtherThermo>::getLocalThermo
(
  const word& /*speciesName*/,
  const pureMixture<ThermoType>& globalThermo
) const
{
  return globalThermo.cellMixture(0);
}
// Constructors 
template<class Thermo, class OtherThermo>
mousse::InterfaceCompositionModel<Thermo, OtherThermo>::InterfaceCompositionModel
(
  const dictionary& dict,
  const phasePair& pair
)
:
  interfaceCompositionModel{dict, pair},
  thermo_
  {
    pair.phase1().mesh().lookupObject<Thermo>
    (
      IOobject::groupName(basicThermo::dictName, pair.phase1().name())
    )
  },
  otherThermo_
  {
    pair.phase2().mesh().lookupObject<OtherThermo>
    (
      IOobject::groupName(basicThermo::dictName, pair.phase2().name())
    )
  },
  Le_{"Le", dimless, dict}
{}
// Destructor 
template<class Thermo, class OtherThermo>
mousse::InterfaceCompositionModel<Thermo, OtherThermo>::
~InterfaceCompositionModel()
{}
// Member Functions 
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::InterfaceCompositionModel<Thermo, OtherThermo>::dY
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  return
    Yf(speciesName, Tf)
    - thermo_.composition().Y()
    [
      thermo_.composition().species()[speciesName]
    ];
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::InterfaceCompositionModel<Thermo, OtherThermo>::D
(
  const word& speciesName
) const
{
  const typename Thermo::thermoType& lTh =
    getLocalThermo
    (
      speciesName,
      thermo_
    );
  const volScalarField& p = thermo_.p();
  const volScalarField& T = thermo_.T();
  tmp<volScalarField> tmpD
  {
    new volScalarField
    {
      {
        IOobject::groupName("D", pair_.name()),
        p.time().timeName(),
        p.mesh()
      },
      p.mesh(),
      {"zero", dimArea/dimTime, 0}
    }
  };
  volScalarField& D = tmpD();
  FOR_ALL(p, cellI)
  {
    D[cellI] = lTh.alphah(p[cellI], T[cellI])/lTh.rho(p[cellI], T[cellI]);
  }
  D /= Le_;
  return tmpD;
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::InterfaceCompositionModel<Thermo, OtherThermo>::L
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  const typename Thermo::thermoType& lTh =
    getLocalThermo
    (
      speciesName,
      thermo_
    );
  const typename OtherThermo::thermoType& oLTh =
    getLocalThermo
    (
      speciesName,
      otherThermo_
    );
  const volScalarField& p = thermo_.p();
  const volScalarField& otherP = otherThermo_.p();
  tmp<volScalarField> tmpL
  {
    new volScalarField
    {
      {
        IOobject::groupName("L", pair_.name()),
        p.time().timeName(),
        p.mesh()
      },
      p.mesh(),
      {"zero", dimEnergy/dimMass, 0}
    }
  };
  volScalarField& L = tmpL();
  FOR_ALL(p, cellI)
  {
    L[cellI] = lTh.Ha(p[cellI], Tf[cellI]) - oLTh.Ha(otherP[cellI], Tf[cellI]);
  }
  return tmpL;
}
template<class Thermo, class OtherThermo>
void mousse::InterfaceCompositionModel<Thermo, OtherThermo>::addMDotL
(
  const volScalarField& K,
  const volScalarField& Tf,
  volScalarField& mDotL,
  volScalarField& mDotLPrime
) const
{
  FOR_ALL_CONST_ITER(hashedWordList, this->speciesNames_, iter)
  {
    volScalarField rhoKDL
    {
      thermo_.rhoThermo::rho()*K*D(*iter)*L(*iter, Tf)
    };
    mDotL += rhoKDL*dY(*iter, Tf);
    mDotLPrime += rhoKDL*YfPrime(*iter, Tf);
  }
}
