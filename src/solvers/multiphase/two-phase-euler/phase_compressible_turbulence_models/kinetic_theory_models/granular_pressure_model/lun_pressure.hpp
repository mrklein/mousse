// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Lun
// Description
// SourceFiles
//   lun_pressure.cpp
#ifndef LUN_HPP_
#define LUN_HPP_
#include "granular_pressure_model.hpp"
namespace mousse {
namespace kineticTheoryModels {
namespace granularPressureModels {
class Lun
:
  public granularPressureModel
{
public:
  //- Runtime type information
  TYPE_NAME("Lun");
  // Constructors
    //- Construct from components
    Lun(const dictionary& dict);
  //- Destructor
  virtual ~Lun();
  // Member Functions
    tmp<volScalarField> granularPressureCoeff
    (
      const volScalarField& alpha1,
      const volScalarField& g0,
      const volScalarField& rho1,
      const dimensionedScalar& e
    ) const;
    tmp<volScalarField> granularPressureCoeffPrime
    (
      const volScalarField& alpha1,
      const volScalarField& g0,
      const volScalarField& g0prime,
      const volScalarField& rho1,
      const dimensionedScalar& e
    ) const;
};
}  // namespace granularPressureModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
