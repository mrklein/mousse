// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "isothermal_phase_model.hpp"
#include "phase_system.hpp"
// Constructors 
template<class BasePhaseModel>
mousse::IsothermalPhaseModel<BasePhaseModel>::IsothermalPhaseModel
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
mousse::IsothermalPhaseModel<BasePhaseModel>::~IsothermalPhaseModel()
{}
// Member Functions 
template<class BasePhaseModel>
void mousse::IsothermalPhaseModel<BasePhaseModel>::correctThermo()
{}
template<class BasePhaseModel>
mousse::tmp<mousse::fvScalarMatrix>
mousse::IsothermalPhaseModel<BasePhaseModel>::heEqn()
{
  return tmp<fvScalarMatrix>();
}
