#ifndef SOLVERS_MULTIPHASE_TWO_PHASE_EULER_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_RADIAL_MODEL_RADIAL_MODEL_HPP_
#define SOLVERS_MULTIPHASE_TWO_PHASE_EULER_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_RADIAL_MODEL_RADIAL_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::radialModel
// SourceFiles
//   radial_model.cpp
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "dimensioned_types.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
class radialModel
{
protected:
  // Protected data
    const dictionary& dict_;
public:
  //- Runtime type information
  TYPE_NAME("radialModel");
  // Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    radialModel,
    dictionary,
    (
      const dictionary& dict
    ),
    (dict)
  );
  // Constructors
    //- Construct from components
    radialModel(const dictionary& dict);
    //- Disallow default bitwise copy construct
    radialModel(const radialModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const radialModel&) = delete;
  // Selectors
    static autoPtr<radialModel> New
    (
      const dictionary& dict
    );
  //- Destructor
  virtual ~radialModel();
  // Member Functions
    //- Radial distribution function
    virtual tmp<volScalarField> g0
    (
      const volScalarField& alpha,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const = 0;
    //- Derivative of the radial distribution function
    virtual tmp<volScalarField> g0prime
    (
      const volScalarField& alpha,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const = 0;
    virtual bool read()
    {
      return true;
    }
};
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
