// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "he_solid_thermo.hpp"
#include "vol_fields.hpp"


// Private Member Functions 
template<class BasicSolidThermo, class MixtureType>
void mousse::heSolidThermo<BasicSolidThermo, MixtureType>::calculate()
{
  scalarField& TCells = this->T_.internalField();
  const scalarField& hCells = this->he_.internalField();
  const scalarField& pCells = this->p_.internalField();
  scalarField& rhoCells = this->rho_.internalField();
  scalarField& alphaCells = this->alpha_.internalField();
  FOR_ALL(TCells, celli) {
    const typename MixtureType::thermoType& mixture_ =
      this->cellMixture(celli);
    const typename MixtureType::thermoType& volMixture_ =
      this->cellVolMixture(pCells[celli], TCells[celli], celli);
    TCells[celli] =
      mixture_.THE
      (
        hCells[celli],
        pCells[celli],
        TCells[celli]
      );
    rhoCells[celli] = volMixture_.rho(pCells[celli], TCells[celli]);
    alphaCells[celli] =
      volMixture_.kappa(pCells[celli], TCells[celli])
      /mixture_.Cpv(pCells[celli], TCells[celli]);
  }
  FOR_ALL(this->T_.boundaryField(), patchi) {
    fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
    fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
    fvPatchScalarField& prho = this->rho_.boundaryField()[patchi];
    fvPatchScalarField& palpha = this->alpha_.boundaryField()[patchi];
    fvPatchScalarField& ph = this->he_.boundaryField()[patchi];
    if (pT.fixesValue()) {
      FOR_ALL(pT, facei) {
        const typename MixtureType::thermoType& mixture_ =
          this->patchFaceMixture(patchi, facei);
        const typename MixtureType::thermoType& volMixture_ =
          this->patchFaceVolMixture
          (
            pp[facei],
            pT[facei],
            patchi,
            facei
          );
        ph[facei] = mixture_.HE(pp[facei], pT[facei]);
        prho[facei] = volMixture_.rho(pp[facei], pT[facei]);
        palpha[facei] =
          volMixture_.kappa(pp[facei], pT[facei])
          /mixture_.Cpv(pp[facei], pT[facei]);
      }
    } else {
      FOR_ALL(pT, facei) {
        const typename MixtureType::thermoType& mixture_ =
          this->patchFaceMixture(patchi, facei);
        const typename MixtureType::thermoType& volMixture_ =
          this->patchFaceVolMixture
          (
            pp[facei],
            pT[facei],
            patchi,
            facei
          );
        pT[facei] = mixture_.THE(ph[facei], pp[facei] ,pT[facei]);
        prho[facei] = volMixture_.rho(pp[facei], pT[facei]);
        palpha[facei] =
          volMixture_.kappa(pp[facei], pT[facei])
          /mixture_.Cpv(pp[facei], pT[facei]);
      }
    }
  }
}


// Constructors 
template<class BasicSolidThermo, class MixtureType>
mousse::heSolidThermo<BasicSolidThermo, MixtureType>::
heSolidThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  heThermo<BasicSolidThermo, MixtureType>{mesh, phaseName}
{
  calculate();
}


template<class BasicSolidThermo, class MixtureType>
mousse::heSolidThermo<BasicSolidThermo, MixtureType>::
heSolidThermo
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
:
  heThermo<BasicSolidThermo, MixtureType>{mesh, dict, phaseName}
{
  calculate();
}


// Destructor 
template<class BasicSolidThermo, class MixtureType>
mousse::heSolidThermo<BasicSolidThermo, MixtureType>::~heSolidThermo()
{}


// Member Functions 
template<class BasicSolidThermo, class MixtureType>
void mousse::heSolidThermo<BasicSolidThermo, MixtureType>::correct()
{
  if (debug) {
    Info << "entering heSolidThermo<MixtureType>::correct()" << endl;
  }
  calculate();
  if (debug) {
    Info << "exiting heSolidThermo<MixtureType>::correct()" << endl;
  }
}


template<class BasicSolidThermo, class MixtureType>
mousse::tmp<mousse::volVectorField>
mousse::heSolidThermo<BasicSolidThermo, MixtureType>::Kappa() const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volVectorField> tKappa
  {
    new volVectorField
    {
      {
        "Kappa",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      dimEnergy/dimTime/dimLength/dimTemperature
    }
  };
  volVectorField& Kappa = tKappa();
  vectorField& KappaCells = Kappa.internalField();
  const scalarField& TCells = this->T_.internalField();
  const scalarField& pCells = this->p_.internalField();
  FOR_ALL(KappaCells, celli) {
    Kappa[celli] =
      this->cellVolMixture
      (
        pCells[celli],
        TCells[celli],
        celli
      ).Kappa(pCells[celli], TCells[celli]);
  }
  FOR_ALL(Kappa.boundaryField(), patchi) {
    vectorField& Kappap = Kappa.boundaryField()[patchi];
    const scalarField& pT = this->T_.boundaryField()[patchi];
    const scalarField& pp = this->p_.boundaryField()[patchi];
    FOR_ALL(Kappap, facei) {
      Kappap[facei] =
        this->patchFaceVolMixture
        (
          pp[facei],
          pT[facei],
          patchi,
          facei
        ).Kappa(pp[facei], pT[facei]);
    }
  }
  return tKappa;
}


template<class BasicSolidThermo, class MixtureType>
mousse::tmp<mousse::vectorField>
mousse::heSolidThermo<BasicSolidThermo, MixtureType>::Kappa
(
  const label patchi
) const
{
  const scalarField& pp = this->p_.boundaryField()[patchi];
  const scalarField& Tp = this->T_.boundaryField()[patchi];
  tmp<vectorField> tKappa{new vectorField(pp.size())};
  vectorField& Kappap = tKappa();
  FOR_ALL(Tp, facei) {
    Kappap[facei] =
      this->patchFaceVolMixture
      (
        pp[facei],
        Tp[facei],
        patchi,
        facei
      ).Kappa(pp[facei], Tp[facei]);
  }
  return tKappa;
}

