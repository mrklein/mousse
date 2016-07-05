#ifndef SOLVERS_MULTIPHASE_TWO_PHASE_EULER_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_FRICTIONAL_STRESS_MODEL_FRICTIONAL_STRESS_MODEL_HPP_
#define SOLVERS_MULTIPHASE_TWO_PHASE_EULER_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_FRICTIONAL_STRESS_MODEL_FRICTIONAL_STRESS_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::frictionalStressModel
// SourceFiles
//   frictional_stress_model.cpp
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "dimensioned_types.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
class frictionalStressModel
{
protected:
  // Protected data
    //- Reference to higher-level dictionary for re-read
    const dictionary& dict_;
public:
  //- Runtime type information
  TYPE_NAME("frictionalStressModel");
  // Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    frictionalStressModel,
    dictionary,
    (
      const dictionary& dict
    ),
    (dict)
  );
  // Constructors
    //- Construct from components
    frictionalStressModel(const dictionary& dict);
    //- Disallow default bitwise copy construct
    frictionalStressModel(const frictionalStressModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const frictionalStressModel&) = delete;
  // Selectors
    static autoPtr<frictionalStressModel> New
    (
      const dictionary& dict
    );
  //- Destructor
  virtual ~frictionalStressModel();
  // Member Functions
    virtual tmp<volScalarField> frictionalPressure
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const = 0;
    virtual tmp<volScalarField> frictionalPressurePrime
    (
      const volScalarField& alpha1f,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const = 0;
    virtual tmp<volScalarField> nu
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax,
      const volScalarField& pf,
      const volSymmTensorField& D
    ) const = 0;
    virtual bool read() = 0;
};
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
