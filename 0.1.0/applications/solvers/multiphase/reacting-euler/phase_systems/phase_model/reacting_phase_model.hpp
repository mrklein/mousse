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
#ifndef reacting_phase_model_hpp_
#define reacting_phase_model_hpp_
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
