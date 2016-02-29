#ifndef THERMOPHYSICAL_MODELS_BASIC_FLUID_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_FLUID_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fluidThermo
// Description
//   Fundamental fluid thermodynamic properties
// SourceFiles
//   fluid_thermo.cpp
#include "basic_thermo.hpp"
#include "compressible_transport_model.hpp"
namespace mousse
{
class fluidThermo
:
  public basicThermo,
  public compressibleTransportModel
{
public:
  //- Runtime type information
  TYPE_NAME("fluidThermo");
  //- Declare run-time constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    fluidThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    fluidThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Construct from mesh and phase name
    fluidThermo
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName
    );
  //- Selector
  static autoPtr<fluidThermo> New
  (
    const fvMesh&,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~fluidThermo();
  // Member functions
    // Access to thermodynamic state variables
      //- Compressibility [s^2/m^2]
      virtual const volScalarField& psi() const = 0;
    // Access to transport state variables
      //- Dynamic viscosity of mixture [kg/m/s]
      virtual tmp<volScalarField> mu() const = 0;
      //- Dynamic viscosity of mixture for patch [kg/m/s]
      virtual tmp<scalarField> mu(const label patchi) const = 0;
      //- Kinematic viscosity of mixture [m^2/s]
      virtual tmp<volScalarField> nu() const;
      //- Kinematic viscosity of mixture for patch [m^2/s]
      virtual tmp<scalarField> nu(const label patchi) const;
};
}  // namespace mousse
#endif
