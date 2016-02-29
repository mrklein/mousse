// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inert_phase_model.hpp"
#include "phase_system.hpp"
// Constructors 
template<class BasePhaseModel>
mousse::InertPhaseModel<BasePhaseModel>::InertPhaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
:
  BasePhaseModel(fluid, phaseName, index)
{}
// Destructor 
template<class BasePhaseModel>
mousse::InertPhaseModel<BasePhaseModel>::~InertPhaseModel()
{}
// Member Functions 
template<class BasePhaseModel>
mousse::tmp<mousse::fvScalarMatrix>
mousse::InertPhaseModel<BasePhaseModel>::R
(
  volScalarField& Yi
) const
{
  return tmp<fvScalarMatrix>
  (
    new fvScalarMatrix(Yi, dimMass/dimTime)
  );
}
template<class BasePhaseModel>
mousse::tmp<mousse::volScalarField>
mousse::InertPhaseModel<BasePhaseModel>::Sh() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("Sh", this->name()),
        this->mesh().time().timeName(),
        this->mesh()
      ),
      this->mesh(),
      dimensionedScalar("zero", dimEnergy/dimTime/dimVolume, 0),
      zeroGradientFvPatchScalarField::typeName
    )
  );
}
