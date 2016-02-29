#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_MASS_TRANSFER_MODELS_SPHERICAL_MASS_TRANSFER_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_MASS_TRANSFER_MODELS_SPHERICAL_MASS_TRANSFER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::massTransferModels::sphericalMassTransfer
// Description
//   Model which applies an analytical solution for mass transfer from the
//   surface of a sphere to the fluid within the sphere.
// SourceFiles
//   spherical_mass_transfer.cpp
#include "mass_transfer_model.hpp"
namespace mousse
{
class phasePair;
namespace massTransferModels
{
class sphericalMassTransfer
:
  public massTransferModel
{
private:
  // Private data
    //- Lewis number
    const dimensionedScalar Le_;
public:
  //- Runtime type information
  TYPE_NAME("spherical");
  // Constructors
    //- Construct from components
    sphericalMassTransfer
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~sphericalMassTransfer();
  // Member Functions
    //- The implicit mass transfer coefficient
    virtual tmp<volScalarField> K() const;
};
}  // namespace massTransferModels
}  // namespace mousse
#endif
