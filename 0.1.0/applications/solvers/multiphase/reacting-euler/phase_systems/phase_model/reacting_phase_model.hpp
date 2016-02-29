#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_MODEL_REACTING_PHASE_MODEL_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_MODEL_REACTING_PHASE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReactingPhaseModel
// Description
//   Class which represents phases with volumetric reactions. Returns the
//   reaction rate and heat.
// SourceFiles
//   reacting_phase_model.cpp
#include "phase_model.hpp"
namespace mousse
{
template<class BasePhaseModel, class ReactionType>
class ReactingPhaseModel
:
  public BasePhaseModel
{
protected:
  // Protected data
    //- Reaction model
    autoPtr<ReactionType> reaction_;
public:
  // Constructors
    ReactingPhaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
  //- Destructor
  virtual ~ReactingPhaseModel();
  // Member Functions
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Return the species fraction equation
    virtual tmp<fvScalarMatrix> R
    (
      volScalarField& Yi
    ) const;
    //- Return the reacton heat source
    virtual tmp<volScalarField> Sh() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "reacting_phase_model.cpp"
#endif
#endif
