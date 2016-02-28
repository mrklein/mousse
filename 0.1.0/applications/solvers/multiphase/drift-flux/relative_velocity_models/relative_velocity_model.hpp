// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::relativeVelocityModel
// Description
// SourceFiles
//   relative_velocity_model.cpp
#ifndef RELATIVE_VELOCITY_MODEL_HPP_
#define RELATIVE_VELOCITY_MODEL_HPP_
#include "fv_cfd.hpp"
#include "dictionary.hpp"
#include "incompressible_two_phase_interacting_mixture.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class relativeVelocityModel
{
protected:
  // Protected data
    //- Mixture properties
    const incompressibleTwoPhaseInteractingMixture& mixture_;
    //- Name of the continuous phase
    const word continuousPhaseName_;
    //- Continuous phase fraction
    const volScalarField& alphac_;
    //- Dispersed phase fraction
    const volScalarField& alphad_;
    //- Continuous density
    const dimensionedScalar& rhoc_;
    //- Dispersed density
    const dimensionedScalar& rhod_;
    //- Dispersed diffusion velocity
    mutable volVectorField Udm_;
public:
  //- Runtime type information
  TYPE_NAME("relativeVelocityModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    relativeVelocityModel,
    dictionary,
    (const dictionary& dict,
    const incompressibleTwoPhaseInteractingMixture& mixture),
    (dict, mixture)
  );
  // Constructors
    //- Construct from components
    relativeVelocityModel
    (
      const dictionary& dict,
      const incompressibleTwoPhaseInteractingMixture& mixture
    );
    //- Disallow default bitwise copy construct
    relativeVelocityModel(const relativeVelocityModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const relativeVelocityModel&) = delete;
  // Selector
  static autoPtr<relativeVelocityModel> New
  (
    const dictionary& dict,
    const incompressibleTwoPhaseInteractingMixture& mixture
  );
  //- Destructor
  virtual ~relativeVelocityModel();
  // Member Functions
    //- Mixture properties
    const incompressibleTwoPhaseInteractingMixture& mixture() const
    {
      return mixture_;
    }
    //- Return the mixture mean density
    tmp<volScalarField> rho() const;
    //- Return the diffusion velocity of the dispersed phase
    const volVectorField& Udm() const
    {
      return Udm_;
    }
    //- Return the stress tensor due to the phase transport
    tmp<volSymmTensorField> tauDm() const;
    //- Update the diffusion velocity
    virtual void correct() = 0;
};
}  // namespace mousse
#endif
