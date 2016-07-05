// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::frictionalStressModels::JohnsonJackson
// Description
// SourceFiles
//   johnson_jackson_frictional_stress.cpp
#ifndef JOHNSON_JACKSON_HPP_
#define JOHNSON_JACKSON_HPP_
#include "frictional_stress_model.hpp"
namespace mousse {
namespace kineticTheoryModels {
namespace frictionalStressModels {
class JohnsonJackson
:
  public frictionalStressModel
{
  // Private data
    dictionary coeffDict_;
    //- Material constant for frictional normal stress
    dimensionedScalar Fr_;
    //- Material constant for frictional normal stress
    dimensionedScalar eta_;
    //- Material constant for frictional normal stress
    dimensionedScalar p_;
    //- Angle of internal friction
    dimensionedScalar phi_;
    //- Lower limit for (alphaMax - alpha1)
    dimensionedScalar alphaDeltaMin_;
public:
  //- Runtime type information
  TYPE_NAME("JohnsonJackson");
  // Constructors
    //- Construct from components
    JohnsonJackson(const dictionary& dict);
  //- Destructor
  virtual ~JohnsonJackson();
  // Member functions
    virtual tmp<volScalarField> frictionalPressure
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const;
    virtual tmp<volScalarField> frictionalPressurePrime
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const;
    virtual tmp<volScalarField> nu
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax,
      const volScalarField& pf,
      const volSymmTensorField& D
    ) const;
    virtual bool read();
};
}  // namespace frictionalStressModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
