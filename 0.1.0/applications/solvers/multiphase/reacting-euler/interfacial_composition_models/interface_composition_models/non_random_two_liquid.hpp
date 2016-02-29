#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_INTERFACE_COMPOSITION_MODELS_NON_RANDOM_TWO_LIQUID_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_INTERFACE_COMPOSITION_MODELS_NON_RANDOM_TWO_LIQUID_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NonRandomTwoLiquid
// Description
//   Non ideal law for the mixing of two species. A separate composition model
//   is given for each species. The composition of a species is equal to the
//   value given by the model, scaled by the species fraction in the bulk of the
//   other phase, and multiplied by the activity coefficient for that species.
//   The gas behaviour is assumed ideal; i.e. the fugacity coefficient is taken
//   as equal to 1.
// SourceFiles
//   non_random_two_liquid.cpp
#include "_interface_composition_model.hpp"
#include "saturation_model.hpp"
namespace mousse
{
class phasePair;
namespace interfaceCompositionModels
{
template<class Thermo, class OtherThermo>
class NonRandomTwoLiquid
:
  public InterfaceCompositionModel<Thermo, OtherThermo>
{
private:
  // Private data
    //- Activity coefficient for species 1
    volScalarField gamma1_;
    //- Activity coefficient for species 2
    volScalarField gamma2_;
    //- Name of species 1
    word species1Name_;
    //- Name of species 2
    word species2Name_;
    //- Indiex of species 1 within this thermo
    label species1Index_;
    //- Indiex of species 2 within this thermo
    label species2Index_;
    //- Non-randomness constant parameter for species 1
    dimensionedScalar alpha12_;
    //- Non-randomness constant parameter for species 2
    dimensionedScalar alpha21_;
    //- Non-randomness linear paramater for species 1
    dimensionedScalar beta12_;
    //- Non-randomness linear paramater for species 2
    dimensionedScalar beta21_;
    //- Interaction parameter model for species 1
    autoPtr<saturationModel> saturationModel12_;
    //- Interaction parameter model for species 2
    autoPtr<saturationModel> saturationModel21_;
    //- Composition model for species 1
    autoPtr<interfaceCompositionModel> speciesModel1_;
    //- Composition model for species 2
    autoPtr<interfaceCompositionModel> speciesModel2_;
public:
  //- Runtime type information
  TYPE_NAME("NonRandomTwoLiquid");
  // Constructors
    //- Construct from components
    NonRandomTwoLiquid
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~NonRandomTwoLiquid();
  // Member Functions
    //- Update the composition
    virtual void update(const volScalarField& Tf);
    //- The interface species fraction
    virtual tmp<volScalarField> Yf
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const;
    //- The interface species fraction derivative w.r.t. temperature
    virtual tmp<volScalarField> YfPrime
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const;
};
}  // namespace interfaceCompositionModels
}  // namespace mousse
#ifdef NoRepository
#   include "non_random_two_liquid.cpp"
#endif
#endif
