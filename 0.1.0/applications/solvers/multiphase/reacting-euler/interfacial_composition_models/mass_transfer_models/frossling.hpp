#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_MASS_TRANSFER_MODELS_FROSSLING_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_MASS_TRANSFER_MODELS_FROSSLING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::massTransferModels::Frossling
// Description
//   Frossling correlation for turbulent mass transfer from the surface of a
//   sphere to the surrounding fluid.
// SourceFiles
//   frossling.cpp
#include "mass_transfer_model.hpp"
namespace mousse
{
class phasePair;
namespace massTransferModels
{
class Frossling
:
  public massTransferModel
{
private:
  // Private data
    //- Lewis number
    const dimensionedScalar Le_;
public:
  //- Runtime type information
  TYPE_NAME("Frossling");
  // Constructors
    //- Construct from components
    Frossling
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Frossling();
  // Member Functions
    //- The implicit mass transfer coefficient
    virtual tmp<volScalarField> K() const;
};
}  // namespace massTransferModels
}  // namespace mousse
#endif
