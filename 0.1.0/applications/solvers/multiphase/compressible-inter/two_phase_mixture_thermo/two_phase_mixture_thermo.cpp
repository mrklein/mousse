// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "two_phase_mixture_thermo.hpp"
#include "gradient_energy_fv_patch_scalar_field.hpp"
#include "mixed_energy_fv_patch_scalar_field.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(twoPhaseMixtureThermo, 0);
}

// Constructors 
mousse::twoPhaseMixtureThermo::twoPhaseMixtureThermo
(
  const fvMesh& mesh
)
:
  psiThermo{mesh, word::null},
  twoPhaseMixture{mesh, *this},
  thermo1_{NULL},
  thermo2_{NULL}
{
  {
    volScalarField T1{IOobject::groupName("T", phase1Name()), T_};
    T1.write();
  }
  {
    volScalarField T2{IOobject::groupName("T", phase2Name()), T_};
    T2.write();
  }
  thermo1_ = rhoThermo::New(mesh, phase1Name());
  thermo2_ = rhoThermo::New(mesh, phase2Name());
  thermo1_->validate(phase1Name(), "e");
  thermo2_->validate(phase2Name(), "e");
  correct();
}
// Destructor 
mousse::twoPhaseMixtureThermo::~twoPhaseMixtureThermo()
{}
// Member Functions 
void mousse::twoPhaseMixtureThermo::correct()
{
  thermo1_->he() = thermo1_->he(p_, T_);
  thermo1_->correct();
  thermo2_->he() = thermo2_->he(p_, T_);
  thermo2_->correct();
  psi_ = alpha1()*thermo1_->psi() + alpha2()*thermo2_->psi();
  mu_ = alpha1()*thermo1_->mu() + alpha2()*thermo2_->mu();
  alpha_ = alpha1()*thermo1_->alpha() + alpha2()*thermo2_->alpha();
}
bool mousse::twoPhaseMixtureThermo::incompressible() const
{
  return thermo1_->incompressible() && thermo2_->incompressible();
}
bool mousse::twoPhaseMixtureThermo::isochoric() const
{
  return thermo1_->isochoric() && thermo2_->isochoric();
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::he
(
  const volScalarField& p,
  const volScalarField& T
) const
{
  return alpha1()*thermo1_->he(p, T) + alpha2()*thermo2_->he(p, T);
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::he
(
  const scalarField& p,
  const scalarField& T,
  const labelList& cells
) const
{
  return
    scalarField(alpha1(), cells)*thermo1_->he(p, T, cells)
    + scalarField(alpha2(), cells)*thermo2_->he(p, T, cells);
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::he
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->he(p, T, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->he(p, T, patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::hc() const
{
  return alpha1()*thermo1_->hc() + alpha2()*thermo2_->hc();
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::THE
(
  const scalarField& /*h*/,
  const scalarField& /*p*/,
  const scalarField& T0,
  const labelList& /*cells*/
) const
{
  NOT_IMPLEMENTED("twoPhaseMixtureThermo::THE(...)");
  return T0;
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::THE
(
  const scalarField& /*h*/,
  const scalarField& /*p*/,
  const scalarField& T0,
  const label /*patchi*/
) const
{
  NOT_IMPLEMENTED("twoPhaseMixtureThermo::THE(...)");
  return T0;
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::Cp() const
{
  return alpha1()*thermo1_->Cp() + alpha2()*thermo2_->Cp();
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::Cp
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->Cp(p, T, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->Cp(p, T, patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::Cv() const
{
  return alpha1()*thermo1_->Cv() + alpha2()*thermo2_->Cv();
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::Cv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->Cv(p, T, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->Cv(p, T, patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::gamma() const
{
  return alpha1()*thermo1_->gamma() + alpha2()*thermo2_->gamma();
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::gamma
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->gamma(p, T, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->gamma(p, T, patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::Cpv() const
{
  return alpha1()*thermo1_->Cpv() + alpha2()*thermo2_->Cpv();
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::Cpv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->Cpv(p, T, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->Cpv(p, T, patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::CpByCpv() const
{
  return
    alpha1()*thermo1_->CpByCpv()
    + alpha2()*thermo2_->CpByCpv();
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::CpByCpv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->CpByCpv(p, T, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->CpByCpv(p, T, patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::nu() const
{
  return mu()/(alpha1()*thermo1_->rho() + alpha2()*thermo2_->rho());
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::nu
(
  const label patchi
) const
{
  return
    mu(patchi)/(alpha1().boundaryField()[patchi]*thermo1_->rho(patchi)
                + alpha2().boundaryField()[patchi]*thermo2_->rho(patchi));
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::kappa() const
{
  return alpha1()*thermo1_->kappa() + alpha2()*thermo2_->kappa();
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::kappa
(
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->kappa(patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->kappa(patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::kappaEff
(
  const volScalarField& alphat
) const
{
  return
    alpha1()*thermo1_->kappaEff(alphat)
    + alpha2()*thermo2_->kappaEff(alphat);
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::kappaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->kappaEff(alphat, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->kappaEff(alphat, patchi);
}
mousse::tmp<mousse::volScalarField> mousse::twoPhaseMixtureThermo::alphaEff
(
  const volScalarField& alphat
) const
{
  return
    alpha1()*thermo1_->alphaEff(alphat)
    + alpha2()*thermo2_->alphaEff(alphat);
}
mousse::tmp<mousse::scalarField> mousse::twoPhaseMixtureThermo::alphaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  return
    alpha1().boundaryField()[patchi]*thermo1_->alphaEff(alphat, patchi)
    + alpha2().boundaryField()[patchi]*thermo2_->alphaEff(alphat, patchi);
}
