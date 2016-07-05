// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multi_component_phase_model.hpp"
#include "phase_system.hpp"
#include "fvm_ddt.hpp"
#include "fvm_div.hpp"
#include "fvm_sup.hpp"
#include "fvm_laplacian.hpp"
#include "fvc_ddt.hpp"
#include "fvc_div.hpp"
#include "fv_matrix.hpp"
// Constructors 
template<class BasePhaseModel>
mousse::MultiComponentPhaseModel<BasePhaseModel>::MultiComponentPhaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
:
  BasePhaseModel(fluid, phaseName, index),
  Sc_
  (
    "Sc",
    dimless,
    fluid.subDict(phaseName)
  ),
  residualAlpha_
  (
    "residualAlpha",
    dimless,
    fluid.mesh().solverDict("Yi")
  ),
  inertIndex_(-1)
{
  const word inertSpecie
  (
    this->thermo_->lookupOrDefault("inertSpecie", word::null)
  );
  if (inertSpecie != word::null)
  {
    inertIndex_ = this->thermo_->composition().species()[inertSpecie];
  }
}
// Destructor 
template<class BasePhaseModel>
mousse::MultiComponentPhaseModel<BasePhaseModel>::~MultiComponentPhaseModel()
{}
// Member Functions 
template<class BasePhaseModel>
void mousse::MultiComponentPhaseModel<BasePhaseModel>::correctThermo()
{
  volScalarField Yt
  {
    {
      IOobject::groupName("Yt", this->name()),
      this->fluid().mesh().time().timeName(),
      this->fluid().mesh()
    },
    this->fluid().mesh(),
    {"zero", dimless, 0}
  };
  PtrList<volScalarField>& Yi = Y();
  FOR_ALL(Yi, i)
  {
    if (i != inertIndex_)
    {
      Yt += Yi[i];
    }
  }
  if (inertIndex_ != -1)
  {
    Yi[inertIndex_] = scalar(1) - Yt;
    Yi[inertIndex_].max(0);
  }
  else
  {
    FOR_ALL(Yi, i)
    {
      Yi[i] /= Yt;
      Yi[i].max(0);
    }
  }
  BasePhaseModel::correctThermo();
}
template<class BasePhaseModel>
mousse::tmp<mousse::fvScalarMatrix>
mousse::MultiComponentPhaseModel<BasePhaseModel>::YiEqn
(
  volScalarField& Yi
)
{
  if ((inertIndex_ != -1)
      && (Yi.name() == IOobject::groupName(
          this->thermo_->composition().species()[inertIndex_],
          this->name()))
  )
  {
    return tmp<fvScalarMatrix>();
  }
  const volScalarField& alpha = *this;
  const surfaceScalarField& alphaRhoPhi = this->alphaRhoPhi();
  const volScalarField& rho = this->rho();

  return
  (
    fvm::ddt(alpha, rho, Yi)
    + fvm::div(alphaRhoPhi, Yi, "div(" + alphaRhoPhi.name() + ",Yi)")
    - fvm::Sp(this->continuityError(), Yi)
    - fvm::laplacian
    (
      fvc::interpolate(alpha)
      *fvc::interpolate(this->turbulence().nut()*rho/Sc_),
      Yi
    )
    ==
    this->R(Yi)
    + fvc::ddt(residualAlpha_*rho, Yi)
    - fvm::ddt(residualAlpha_*rho, Yi)
  );
}
template<class BasePhaseModel>
const mousse::PtrList<mousse::volScalarField>&
mousse::MultiComponentPhaseModel<BasePhaseModel>::Y() const
{
  return this->thermo_->composition().Y();
}
template<class BasePhaseModel>
mousse::PtrList<mousse::volScalarField>&
mousse::MultiComponentPhaseModel<BasePhaseModel>::Y()
{
  return this->thermo_->composition().Y();
}
