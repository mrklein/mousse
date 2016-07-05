// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pure_phase_model.hpp"
#include "phase_system.hpp"
// Constructors 
template<class BasePhaseModel>
mousse::PurePhaseModel<BasePhaseModel>::PurePhaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
:
  BasePhaseModel{fluid, phaseName, index}
{}
// Destructor 
template<class BasePhaseModel>
mousse::PurePhaseModel<BasePhaseModel>::~PurePhaseModel()
{}
// Member Functions 
template<class BasePhaseModel>
mousse::tmp<mousse::fvScalarMatrix>
mousse::PurePhaseModel<BasePhaseModel>::YiEqn
(
  volScalarField& /*Yi*/
)
{
  NOT_IMPLEMENTED
  (
    "template<class BasePhaseModel> "
    "mousse::tmp<mousse::fvScalarMatrix> "
    "mousse::PurePhaseModel<BasePhaseModel>::YiEqn"
    "(volScalarField& Yi) const"
  );
  return tmp<fvScalarMatrix>();
}
template<class BasePhaseModel>
const mousse::PtrList<mousse::volScalarField>&
mousse::PurePhaseModel<BasePhaseModel>::Y() const
{
  return Y_;
}
template<class BasePhaseModel>
mousse::PtrList<mousse::volScalarField>&
mousse::PurePhaseModel<BasePhaseModel>::Y()
{
  return Y_;
}
