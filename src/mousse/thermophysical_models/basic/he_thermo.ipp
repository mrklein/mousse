// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "he_thermo.hpp"
#include "gradient_energy_fv_patch_scalar_field.hpp"
#include "mixed_energy_fv_patch_scalar_field.hpp"


// Protected Member Functions 
template<class BasicThermo, class MixtureType>
void mousse::heThermo<BasicThermo, MixtureType>::
heBoundaryCorrection(volScalarField& h)
{
  volScalarField::GeometricBoundaryField& hbf = h.boundaryField();
  FOR_ALL(hbf, patchi) {
    if (isA<gradientEnergyFvPatchScalarField>(hbf[patchi])) {
      refCast<gradientEnergyFvPatchScalarField>(hbf[patchi]).gradient()
        = hbf[patchi].fvPatchField::snGrad();
    } else if (isA<mixedEnergyFvPatchScalarField>(hbf[patchi])) {
      refCast<mixedEnergyFvPatchScalarField>(hbf[patchi]).refGrad()
        = hbf[patchi].fvPatchField::snGrad();
    }
  }
}


template<class BasicThermo, class MixtureType>
void mousse::heThermo<BasicThermo, MixtureType>::init()
{
  scalarField& heCells = he_.internalField();
  const scalarField& pCells = this->p_.internalField();
  const scalarField& TCells = this->T_.internalField();
  FOR_ALL(heCells, celli) {
    heCells[celli] =
      this->cellMixture(celli).HE(pCells[celli], TCells[celli]);
  }
  FOR_ALL(he_.boundaryField(), patchi) {
    he_.boundaryField()[patchi] ==
      he(this->p_.boundaryField()[patchi], this->T_.boundaryField()[patchi],
         patchi);
  }
  this->heBoundaryCorrection(he_);
}


// Constructors 
template<class BasicThermo, class MixtureType>
mousse::heThermo<BasicThermo, MixtureType>::heThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  BasicThermo{mesh, phaseName},
  MixtureType{*this, mesh, phaseName},
  he_
  {
    {
      BasicThermo::phasePropertyName(MixtureType::thermoType::heName()),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    dimEnergy/dimMass,
    this->heBoundaryTypes(),
    this->heBoundaryBaseTypes()
  }
{
  init();
}


template<class BasicThermo, class MixtureType>
mousse::heThermo<BasicThermo, MixtureType>::heThermo
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
:
  BasicThermo{mesh, dict, phaseName},
  MixtureType{*this, mesh, phaseName},
  he_
  {
    {
      BasicThermo::phasePropertyName(MixtureType::thermoType::heName()),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    dimEnergy/dimMass,
    this->heBoundaryTypes(),
    this->heBoundaryBaseTypes()
  }
{
  init();
}


// Destructor 
template<class BasicThermo, class MixtureType>
mousse::heThermo<BasicThermo, MixtureType>::~heThermo()
{}


// Member Functions 
template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField> mousse::heThermo<BasicThermo, MixtureType>::he
(
  const volScalarField& p,
  const volScalarField& T
) const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volScalarField> the
  {
    new volScalarField
    {
      {
        "he",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      he_.dimensions()
    }
  };
  volScalarField& he = the();
  scalarField& heCells = he.internalField();
  const scalarField& pCells = p.internalField();
  const scalarField& TCells = T.internalField();
  FOR_ALL(heCells, celli) {
    heCells[celli] =
      this->cellMixture(celli).HE(pCells[celli], TCells[celli]);
  }
  FOR_ALL(he.boundaryField(), patchi) {
    scalarField& hep = he.boundaryField()[patchi];
    const scalarField& pp = p.boundaryField()[patchi];
    const scalarField& Tp = T.boundaryField()[patchi];
    FOR_ALL(hep, facei) {
      hep[facei] =
        this->patchFaceMixture(patchi, facei).HE(pp[facei], Tp[facei]);
    }
  }
  return the;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::he
(
  const scalarField& p,
  const scalarField& T,
  const labelList& cells
) const
{
  tmp<scalarField> the{new scalarField{T.size()}};
  scalarField& he = the();
  FOR_ALL(T, celli) {
    he[celli] = this->cellMixture(cells[celli]).HE(p[celli], T[celli]);
  }
  return the;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::he
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  tmp<scalarField> the{new scalarField{T.size()}};
  scalarField& he = the();
  FOR_ALL(T, facei) {
    he[facei] = this->patchFaceMixture(patchi, facei).HE(p[facei], T[facei]);
  }
  return the;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::hc() const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volScalarField> thc
  {
    new volScalarField
    {
      {
        "hc",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      he_.dimensions()
    }
  };
  volScalarField& hcf = thc();
  scalarField& hcCells = hcf.internalField();
  FOR_ALL(hcCells, celli) {
    hcCells[celli] = this->cellMixture(celli).Hc();
  }
  FOR_ALL(hcf.boundaryField(), patchi) {
    scalarField& hcp = hcf.boundaryField()[patchi];
    FOR_ALL(hcp, facei) {
      hcp[facei] = this->patchFaceMixture(patchi, facei).Hc();
    }
  }
  return thc;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::Cp
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  tmp<scalarField> tCp{new scalarField{T.size()}};
  scalarField& cp = tCp();
  FOR_ALL(T, facei) {
    cp[facei] = this->patchFaceMixture(patchi, facei).Cp(p[facei], T[facei]);
  }
  return tCp;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::Cp() const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volScalarField> tCp
  {
    new volScalarField
    {
      {
        "Cp",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      dimEnergy/dimMass/dimTemperature
    }
  };
  volScalarField& cp = tCp();
  FOR_ALL(this->T_, celli) {
    cp[celli] = this->cellMixture(celli).Cp(this->p_[celli], this->T_[celli]);
  }
  FOR_ALL(this->T_.boundaryField(), patchi) {
    const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
    const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
    fvPatchScalarField& pCp = cp.boundaryField()[patchi];
    FOR_ALL(pT, facei) {
      pCp[facei] =
        this->patchFaceMixture(patchi, facei).Cp(pp[facei], pT[facei]);
    }
  }
  return tCp;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField>
mousse::heThermo<BasicThermo, MixtureType>::Cv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  tmp<scalarField> tCv{new scalarField{T.size()}};
  scalarField& cv = tCv();
  FOR_ALL(T, facei) {
    cv[facei] = this->patchFaceMixture(patchi, facei).Cv(p[facei], T[facei]);
  }
  return tCv;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::Cv() const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volScalarField> tCv
  {
    new volScalarField
    {
      {
        "Cv",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      dimEnergy/dimMass/dimTemperature
    }
  };
  volScalarField& cv = tCv();
  FOR_ALL(this->T_, celli) {
    cv[celli] =
      this->cellMixture(celli).Cv(this->p_[celli], this->T_[celli]);
  }
  FOR_ALL(this->T_.boundaryField(), patchi) {
    cv.boundaryField()[patchi] =
      Cv
      (
        this->p_.boundaryField()[patchi],
        this->T_.boundaryField()[patchi],
        patchi
      );
  }
  return tCv;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::gamma
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  tmp<scalarField> tgamma{new scalarField{T.size()}};
  scalarField& cpv = tgamma();
  FOR_ALL(T, facei) {
    cpv[facei] =
      this->patchFaceMixture(patchi, facei).gamma(p[facei], T[facei]);
  }
  return tgamma;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::gamma() const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volScalarField> tgamma
  {
    new volScalarField
    {
      {
        "gamma",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      dimless
    }
  };
  volScalarField& cpv = tgamma();
  FOR_ALL(this->T_, celli) {
    cpv[celli] =
      this->cellMixture(celli).gamma(this->p_[celli], this->T_[celli]);
  }
  FOR_ALL(this->T_.boundaryField(), patchi) {
    const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
    const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
    fvPatchScalarField& pgamma = cpv.boundaryField()[patchi];
    FOR_ALL(pT, facei) {
      pgamma[facei] =
        this->patchFaceMixture(patchi, facei).gamma
        (
          pp[facei],
          pT[facei]
        );
    }
  }
  return tgamma;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::Cpv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  tmp<scalarField> tCpv{new scalarField{T.size()}};
  scalarField& cpv = tCpv();
  FOR_ALL(T, facei) {
    cpv[facei] = this->patchFaceMixture(patchi, facei).Cpv(p[facei], T[facei]);
  }
  return tCpv;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::Cpv() const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volScalarField> tCpv
  {
    new volScalarField
    {
      {
        "Cpv",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      dimEnergy/dimMass/dimTemperature
    }
  };
  volScalarField& cpv = tCpv();
  FOR_ALL(this->T_, celli) {
    cpv[celli] =
      this->cellMixture(celli).Cpv(this->p_[celli], this->T_[celli]);
  }
  FOR_ALL(this->T_.boundaryField(), patchi) {
    const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
    const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
    fvPatchScalarField& pCpv = cpv.boundaryField()[patchi];
    FOR_ALL(pT, facei) {
      pCpv[facei] =
        this->patchFaceMixture(patchi, facei).Cpv(pp[facei], pT[facei]);
    }
  }
  return tCpv;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::CpByCpv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  tmp<scalarField> tCpByCpv{new scalarField{T.size()}};
  scalarField& cpByCpv = tCpByCpv();
  FOR_ALL(T, facei) {
    cpByCpv[facei] =
      this->patchFaceMixture(patchi, facei).cpBycpv(p[facei], T[facei]);
  }
  return tCpByCpv;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::CpByCpv() const
{
  const fvMesh& mesh = this->T_.mesh();
  tmp<volScalarField> tCpByCpv
  {
    new volScalarField
    {
      {
        "CpByCpv",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      dimless
    }
  };
  volScalarField& cpByCpv = tCpByCpv();
  FOR_ALL(this->T_, celli) {
    cpByCpv[celli] =
      this->cellMixture(celli).cpBycpv
      (
        this->p_[celli],
        this->T_[celli]
      );
  }
  FOR_ALL(this->T_.boundaryField(), patchi) {
    const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
    const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
    fvPatchScalarField& pCpByCpv = cpByCpv.boundaryField()[patchi];
    FOR_ALL(pT, facei) {
      pCpByCpv[facei] =
        this->patchFaceMixture(patchi, facei).cpBycpv
        (
          pp[facei],
          pT[facei]
        );
    }
  }
  return tCpByCpv;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::THE
(
  const scalarField& h,
  const scalarField& p,
  const scalarField& T0,
  const labelList& cells
) const
{
  tmp<scalarField> tT{new scalarField{h.size()}};
  scalarField& T = tT();
  FOR_ALL(h, celli) {
    T[celli] =
      this->cellMixture(cells[celli]).THE(h[celli], p[celli], T0[celli]);
  }
  return tT;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::THE
(
  const scalarField& h,
  const scalarField& p,
  const scalarField& T0,
  const label patchi
) const
{
  tmp<scalarField> tT{new scalarField{h.size()}};
  scalarField& T = tT();
  FOR_ALL(h, facei) {
    T[facei] =
      this->patchFaceMixture(patchi, facei)
        .THE(h[facei], p[facei], T0[facei]);
  }
  return tT;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::kappa() const
{
  tmp<mousse::volScalarField> kappa{Cp()*this->alpha_};
  kappa().rename("kappa");
  return kappa;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField> mousse::heThermo<BasicThermo, MixtureType>::kappa
(
  const label patchi
) const
{
  return
    Cp(this->p_.boundaryField()[patchi], this->T_.boundaryField()[patchi],
       patchi)*this->alpha_.boundaryField()[patchi];
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::kappaEff
(
  const volScalarField& alphat
) const
{
  tmp<mousse::volScalarField> kappaEff{Cp()*(this->alpha_ + alphat)};
  kappaEff().rename("kappaEff");
  return kappaEff;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField>
mousse::heThermo<BasicThermo, MixtureType>::kappaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  return
    Cp(this->p_.boundaryField()[patchi], this->T_.boundaryField()[patchi],
       patchi)*(this->alpha_.boundaryField()[patchi] + alphat);
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::volScalarField>
mousse::heThermo<BasicThermo, MixtureType>::alphaEff
(
  const volScalarField& alphat
) const
{
  tmp<mousse::volScalarField> alphaEff{this->CpByCpv()*(this->alpha_ + alphat)};
  alphaEff().rename("alphaEff");
  return alphaEff;
}


template<class BasicThermo, class MixtureType>
mousse::tmp<mousse::scalarField>
mousse::heThermo<BasicThermo, MixtureType>::alphaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  return
    this->CpByCpv
    (
      this->p_.boundaryField()[patchi],
      this->T_.boundaryField()[patchi],
      patchi
    )*(this->alpha_.boundaryField()[patchi] + alphat);
}


template<class BasicThermo, class MixtureType>
bool mousse::heThermo<BasicThermo, MixtureType>::read()
{
  if (BasicThermo::read()) {
    MixtureType::read(*this);
    return true;
  } else {
    return false;
  }
}

