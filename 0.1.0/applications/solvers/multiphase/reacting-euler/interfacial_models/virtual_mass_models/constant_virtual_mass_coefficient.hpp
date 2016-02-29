#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_VIRTUAL_MASS_MODELS_CONSTANT_VIRTUAL_MASS_COEFFICIENT_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_VIRTUAL_MASS_MODELS_CONSTANT_VIRTUAL_MASS_COEFFICIENT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::virtualMassModels::constantVirtualMassCoefficient
// Description
//   Constant coefficient virtual mass model.
// SourceFiles
//   constant_virtual_mass_coefficient.cpp
#include "virtual_mass_model.hpp"
namespace mousse
{
class phasePair;
namespace virtualMassModels
{
class constantVirtualMassCoefficient
:
  public virtualMassModel
{
  // Private data
    //- Constant virtual mass coefficient
    const dimensionedScalar Cvm_;
public:
  //- Runtime type information
  TYPE_NAME("constantCoefficient");
  // Constructors
    //- Construct from a dictionary and a phase pair
    constantVirtualMassCoefficient
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~constantVirtualMassCoefficient();
  // Member Functions
    //- Virtual mass coefficient
    virtual tmp<volScalarField> Cvm() const;
};
}  // namespace virtualMassModels
}  // namespace mousse
#endif
