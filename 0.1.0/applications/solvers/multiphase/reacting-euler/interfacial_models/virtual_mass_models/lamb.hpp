#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_VIRTUAL_MASS_MODELS_LAMB_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_VIRTUAL_MASS_MODELS_LAMB_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::virtualMassModels::Lamb
// Description
//   Virtual mass model of Lamb.
//   Reference:
//   \verbatim
//     "Hydrodynamics"
//     Lamb, H.,
//     Cambridge University Press, 1895
//   \endverbatim
// SourceFiles
//   lamb.cpp
#include "virtual_mass_model.hpp"
namespace mousse
{
class phasePair;
namespace virtualMassModels
{
class Lamb
:
  public virtualMassModel
{
public:
  //- Runtime type information
  TYPE_NAME("Lamb");
  // Constructors
    //- Construct from a dictionary and a phase pair
    Lamb
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~Lamb();
  // Member Functions
    //- Virtual mass coefficient
    virtual tmp<volScalarField> Cvm() const;
};
}  // namespace virtualMassModels
}  // namespace mousse
#endif
