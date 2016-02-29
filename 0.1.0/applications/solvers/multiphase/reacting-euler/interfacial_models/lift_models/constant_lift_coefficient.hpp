#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_LIFT_MODELS_CONSTANT_LIFT_COEFFICIENT_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_LIFT_MODELS_CONSTANT_LIFT_COEFFICIENT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liftModels::constantLiftCoefficient
// Description
//   Constant coefficient lift model.
// SourceFiles
//   constant_lift_coefficient.cpp
#include "lift_model.hpp"
namespace mousse
{
class phasePair;
namespace liftModels
{
class constantLiftCoefficient
:
  public liftModel
{
  // Private data
    //- Constant lift coefficient
    const dimensionedScalar Cl_;
public:
  //- Runtime type information
  TYPE_NAME("constantCoefficient");
  // Constructors
    //- Construct from a dictionary and a phase pair
    constantLiftCoefficient
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~constantLiftCoefficient();
  // Member Functions
    //- Lift coefficient
    virtual tmp<volScalarField> Cl() const;
};
}  // namespace liftModels
}  // namespace mousse
#endif
