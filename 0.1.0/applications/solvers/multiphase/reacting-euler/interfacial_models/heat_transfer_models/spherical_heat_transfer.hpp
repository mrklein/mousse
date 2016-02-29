#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_HEAT_TRANSFER_MODELS_SPHERICAL_HEAT_TRANSFER_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_HEAT_TRANSFER_MODELS_SPHERICAL_HEAT_TRANSFER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heatTransferModels::sphericalHeatTransfer
// Description
//   Model which applies an analytical solution for heat transfer from the
//   surface of a sphere to the fluid within the sphere.
// SourceFiles
//   spherical_heat_transfer.cpp
#include "heat_transfer_model.hpp"
namespace mousse
{
class phasePair;
namespace heatTransferModels
{
class sphericalHeatTransfer
:
  public heatTransferModel
{
public:
  //- Runtime type information
  TYPE_NAME("spherical");
  // Constructors
    //- Construct from components
    sphericalHeatTransfer
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~sphericalHeatTransfer();
  // Member Functions
    //- The heat transfer function K used in the enthalpy equation
    tmp<volScalarField> K(const scalar residualAlpha) const;
};
}  // namespace heatTransferModels
}  // namespace mousse
#endif
