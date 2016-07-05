// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_phase_model.hpp"
#include "phase_system.hpp"
#include "fv_matrix.hpp"
// Constructors 
template<class BasePhaseModel, class ReactionType>
mousse::ReactingPhaseModel<BasePhaseModel, ReactionType>::ReactingPhaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
:
  BasePhaseModel(fluid, phaseName, index, false),
  reaction_
  (
    ReactionType::New(fluid.mesh(), this->name())
  )
{
  this->thermo_ = &reaction_->thermo();
  this->thermo_->validate
  (
    IOobject::groupName(phaseModel::typeName, this->name()),
    "h",
    "e"
  );
}
// Destructor 
template<class BasePhaseModel, class ReactionType>
mousse::ReactingPhaseModel<BasePhaseModel, ReactionType>::~ReactingPhaseModel()
{}
// Member Functions 
template<class BasePhaseModel, class ReactionType>
void mousse::ReactingPhaseModel<BasePhaseModel, ReactionType>::correctThermo()
{
  reaction_->setTurbulence
  (
    const_cast<compressibleTurbulenceModel&>
    (
      this->mesh().template lookupObject<compressibleTurbulenceModel>
      (
        IOobject::groupName
        (
          turbulenceModel::propertiesName,
          this->name()
        )
      )
    )
  );
  BasePhaseModel::correctThermo();
  reaction_->correct();
}
template<class BasePhaseModel, class ReactionType>
mousse::tmp<mousse::fvScalarMatrix>
mousse::ReactingPhaseModel<BasePhaseModel, ReactionType>::R
(
  volScalarField& Yi
) const
{
  return reaction_->R(Yi);
}
template<class BasePhaseModel, class ReactionType>
mousse::tmp<mousse::volScalarField>
mousse::ReactingPhaseModel<BasePhaseModel, ReactionType>::Sh() const
{
  return reaction_->Sh();
}
