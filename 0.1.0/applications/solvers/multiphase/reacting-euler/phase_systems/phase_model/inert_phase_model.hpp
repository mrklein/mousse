#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_MODEL_INERT_PHASE_MODEL_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_PHASE_MODEL_INERT_PHASE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InertPhaseModel
// Description
//   Class which represents an inert phase, with no reactions. Returns zero
//   reaction rate and heat.
// SourceFiles
//   inert_phase_model.cpp
#include "phase_model.hpp"
namespace mousse
{
template<class BasePhaseModel>
class InertPhaseModel
:
  public BasePhaseModel
{
public:
  // Constructors
    InertPhaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
  //- Destructor
  virtual ~InertPhaseModel();
  // Member Functions
    // Thermo
      //- Return the fuel consumption rate matrix
      virtual tmp<fvScalarMatrix> R
      (
        volScalarField& Yi
      ) const;
      //- Return the reaction heat source
      virtual tmp<volScalarField> Sh() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "inert_phase_model.cpp"
#endif
#endif
