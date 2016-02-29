#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_MODEL_ISOTHERMAL_PHASE_MODEL_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_MODEL_ISOTHERMAL_PHASE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IsothermalPhaseModel
// Description
//   Class which represents a phase for which the temperature (strictly energy)
//   remains constant. Returns an empty energy equation and does nothing when
//   correctThermo is called.
// SourceFiles
//   isothermal_phase_model.cpp
#include "phase_model.hpp"
namespace mousse
{
template<class BasePhaseModel>
class IsothermalPhaseModel
:
  public BasePhaseModel
{
public:
  // Constructors
    IsothermalPhaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
  //- Destructor
  virtual ~IsothermalPhaseModel();
  // Member Functions
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Return the enthalpy equation
    virtual tmp<fvScalarMatrix> heEqn();
};
}  // namespace mousse
#ifdef NoRepository
#   include "isothermal_phase_model.cpp"
#endif
#endif
