// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SyamlalRogersOBrien
// Description
// SourceFiles
//   syamlal_rogers_o_brien_pressure.cpp
#ifndef SyamlalRogersOBrien_H
#define SyamlalRogersOBrien_H
#include "granular_pressure_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace granularPressureModels
{
class SyamlalRogersOBrien
:
  public granularPressureModel
{
public:
  //- Runtime type information
  TYPE_NAME("SyamlalRogersOBrien");
  // Constructors
    //- Construct from components
    SyamlalRogersOBrien(const dictionary& dict);
  //- Destructor
  virtual ~SyamlalRogersOBrien();
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
