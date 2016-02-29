#ifndef THERMOPHYSICAL_MODELS_BAROTROPIC_COMPRESSIBILITY_MODEL_WALLIS_HPP_
#define THERMOPHYSICAL_MODELS_BAROTROPIC_COMPRESSIBILITY_MODEL_WALLIS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressibilityModels::Wallis
// Description
//   Wallis compressibility model.
// SourceFiles
//   wallis.cpp
#include "barotropic_compressibility_model.hpp"
#include "dimensioned_scalar.hpp"
namespace mousse
{
namespace compressibilityModels
{
class Wallis
:
  public barotropicCompressibilityModel
{
  // Private data
    dimensionedScalar psiv_;
    dimensionedScalar psil_;
    dimensionedScalar rhovSat_;
    dimensionedScalar rholSat_;
public:
  //- Runtime type information
  TYPE_NAME("Wallis");
  // Constructors
    //- Construct from components
    Wallis
    (
      const dictionary& compressibilityProperties,
      const volScalarField& gamma,
      const word& psiName = "psi"
    );
  //- Destructor
  ~Wallis()
  {}
  // Member Functions
    //- Correct the Wallis compressibility
    void correct();
    //- Read transportProperties dictionary
    bool read(const dictionary& compressibilityProperties);
};
}  // namespace compressibilityModels
}  // namespace mousse
#endif
