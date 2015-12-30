// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "he_psi_thermo.hpp"
// Private Member Functions 
template<class BasicPsiThermo, class MixtureType>
void mousse::hePsiThermo<BasicPsiThermo, MixtureType>::calculate()
{
  const scalarField& hCells = this->he_.internalField();
  const scalarField& pCells = this->p_.internalField();
  scalarField& TCells = this->T_.internalField();
  scalarField& psiCells = this->psi_.internalField();
  scalarField& muCells = this->mu_.internalField();
  scalarField& alphaCells = this->alpha_.internalField();
  forAll(TCells, celli)
  {
    const typename MixtureType::thermoType& mixture_ =
      this->cellMixture(celli);
    TCells[celli] = mixture_.THE
    (
      hCells[celli],
      pCells[celli],
      TCells[celli]
    );
    psiCells[celli] = mixture_.psi(pCells[celli], TCells[celli]);
    muCells[celli] = mixture_.mu(pCells[celli], TCells[celli]);
    alphaCells[celli] = mixture_.alphah(pCells[celli], TCells[celli]);
  }
  forAll(this->T_.boundaryField(), patchi)
  {
    fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
    fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
    fvPatchScalarField& ppsi = this->psi_.boundaryField()[patchi];
    fvPatchScalarField& ph = this->he_.boundaryField()[patchi];
    fvPatchScalarField& pmu = this->mu_.boundaryField()[patchi];
    fvPatchScalarField& palpha = this->alpha_.boundaryField()[patchi];
    if (pT.fixesValue())
    {
      forAll(pT, facei)
      {
        const typename MixtureType::thermoType& mixture_ =
          this->patchFaceMixture(patchi, facei);
        ph[facei] = mixture_.HE(pp[facei], pT[facei]);
        ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
        pmu[facei] = mixture_.mu(pp[facei], pT[facei]);
        palpha[facei] = mixture_.alphah(pp[facei], pT[facei]);
      }
    }
    else
    {
      forAll(pT, facei)
      {
        const typename MixtureType::thermoType& mixture_ =
          this->patchFaceMixture(patchi, facei);
        pT[facei] = mixture_.THE(ph[facei], pp[facei], pT[facei]);
        ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
        pmu[facei] = mixture_.mu(pp[facei], pT[facei]);
        palpha[facei] = mixture_.alphah(pp[facei], pT[facei]);
      }
    }
  }
}
// Constructors 
template<class BasicPsiThermo, class MixtureType>
mousse::hePsiThermo<BasicPsiThermo, MixtureType>::hePsiThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  heThermo<BasicPsiThermo, MixtureType>(mesh, phaseName)
{
  calculate();
  // Switch on saving old time
  this->psi_.oldTime();
}
// Destructor 
template<class BasicPsiThermo, class MixtureType>
mousse::hePsiThermo<BasicPsiThermo, MixtureType>::~hePsiThermo()
{}
// Member Functions 
template<class BasicPsiThermo, class MixtureType>
void mousse::hePsiThermo<BasicPsiThermo, MixtureType>::correct()
{
  if (debug)
  {
    Info<< "entering hePsiThermo<BasicPsiThermo, MixtureType>::correct()"
      << endl;
  }
  // force the saving of the old-time values
  this->psi_.oldTime();
  calculate();
  if (debug)
  {
    Info<< "exiting hePsiThermo<BasicPsiThermo, MixtureType>::correct()"
      << endl;
  }
}
