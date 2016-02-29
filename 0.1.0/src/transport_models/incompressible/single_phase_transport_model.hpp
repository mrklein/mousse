#ifndef TRANSPORT_MODELS_INCOMPRESSIBLE_SINGLE_PHASE_TRANSPORT_MODEL_HPP_
#define TRANSPORT_MODELS_INCOMPRESSIBLE_SINGLE_PHASE_TRANSPORT_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::singlePhaseTransportModel
// Description
//   A simple single-phase transport model based on viscosityModel.
//   Used by the incompressible single-phase solvers like mousse-simple,
//   mousse-turb etc.
// SourceFiles
//   single_phase_transport_model.cpp
#include "incompressible/transport_model.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class viscosityModel;
class singlePhaseTransportModel
:
  public IOdictionary,
  public transportModel
{
  // Private Data
    autoPtr<viscosityModel> viscosityModelPtr_;
public:
  // Constructors
    //- Construct from components
    singlePhaseTransportModel
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
    //- Disallow copy construct
    singlePhaseTransportModel(const singlePhaseTransportModel&) = delete;
    //- Disallow default bitwise assignment
    singlePhaseTransportModel& operator=
    (
      const singlePhaseTransportModel&
    ) = delete;
  //- Destructor
  virtual ~singlePhaseTransportModel();
  // Member Functions
    //- Return the laminar viscosity
    virtual tmp<volScalarField> nu() const;
    //- Return the laminar viscosity for patch
    virtual tmp<scalarField> nu(const label patchi) const;
    //- Correct the laminar viscosity
    virtual void correct();
    //- Read transportProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
