#ifndef TURBULENCE_MODELS_PHASE_INCOMPRESSIBLE_PHASE_INCOMPRESSIBLE_TURBULENCE_MODEL_HPP_
#define TURBULENCE_MODELS_PHASE_INCOMPRESSIBLE_PHASE_INCOMPRESSIBLE_TURBULENCE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PhaseIncompressibleTurbulenceModel
// Description
//   Templated abstract base class for multiphase incompressible
//   turbulence models.
// SourceFiles
//   phase_incompressible_turbulence_model.cpp
#include "_turbulence_model.hpp"
#include "incompressible_turbulence_model.hpp"
namespace mousse
{
template<class TransportModel>
class PhaseIncompressibleTurbulenceModel
:
  public TurbulenceModel
  <
    volScalarField,
    geometricOneField,
    incompressibleTurbulenceModel,
    TransportModel
  >
{
public:
  typedef volScalarField alphaField;
  typedef geometricOneField rhoField;
  typedef TransportModel transportModel;
  // Constructors
    //- Construct
    PhaseIncompressibleTurbulenceModel
    (
      const word& type,
      const alphaField& alpha,
      const geometricOneField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const TransportModel& trasportModel,
      const word& propertiesName
    );
  // Selectors
    //- Return a reference to the selected turbulence model
    static autoPtr<PhaseIncompressibleTurbulenceModel> New
    (
      const alphaField& alpha,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const TransportModel& trasportModel,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~PhaseIncompressibleTurbulenceModel()
  {}
  // Member Functions
    //- Return the phase-pressure'
    // (derivative of phase-pressure w.r.t. phase-fraction)
    virtual tmp<volScalarField> pPrime() const;
    //- Return the face-phase-pressure'
    // (derivative of phase-pressure w.r.t. phase-fraction)
    virtual tmp<surfaceScalarField> pPrimef() const;
    //- Return the effective stress tensor
    virtual tmp<volSymmTensorField> devReff() const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;
    //- Return the effective stress tensor
    virtual tmp<volSymmTensorField> devRhoReff() const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "phase_incompressible_turbulence_model.cpp"
#endif
#endif
