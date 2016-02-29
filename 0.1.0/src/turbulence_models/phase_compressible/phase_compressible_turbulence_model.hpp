#ifndef TURBULENCE_MODELS_PHASE_COMPRESSIBLE_PHASE_COMPRESSIBLE_TURBULENCE_MODEL_HPP_
#define TURBULENCE_MODELS_PHASE_COMPRESSIBLE_PHASE_COMPRESSIBLE_TURBULENCE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PhaseCompressibleTurbulenceModel
// Description
//   Templated abstract base class for multiphase compressible
//   turbulence models.
// SourceFiles
//   phase_compressible_turbulence_model.cpp
#include "_turbulence_model.hpp"
#include "compressible_turbulence_model.hpp"
namespace mousse
{
template<class TransportModel>
class PhaseCompressibleTurbulenceModel
:
  public TurbulenceModel
  <
    volScalarField,
    volScalarField,
    compressibleTurbulenceModel,
    TransportModel
  >
{
public:
  typedef volScalarField alphaField;
  typedef volScalarField rhoField;
  typedef TransportModel transportModel;
  // Constructors
    //- Construct
    PhaseCompressibleTurbulenceModel
    (
      const word& type,
      const alphaField& alpha,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& trasport,
      const word& propertiesName
    );
  // Selectors
    //- Return a reference to the selected turbulence model
    static autoPtr<PhaseCompressibleTurbulenceModel> New
    (
      const alphaField& alpha,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& trasportModel,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~PhaseCompressibleTurbulenceModel()
  {}
  // Member Functions
    //- Return the laminar dynamic viscosity
    virtual tmp<volScalarField> mu() const
    {
      return this->transport_.mu();
    }
    //- Return the laminar dynamic viscosity on patch
    virtual tmp<scalarField> mu(const label patchi) const
    {
      return this->transport_.mu(patchi);
    }
    //- Return the turbulence dynamic viscosity
    virtual tmp<volScalarField> mut() const
    {
      return this->rho_*this->nut();
    }
    //- Return the turbulence dynamic viscosity on patch
    virtual tmp<scalarField> mut(const label patchi) const
    {
      return this->rho_.boundaryField()[patchi]*this->nut(patchi);
    }
    //- Return the effective dynamic viscosity
    virtual tmp<volScalarField> muEff() const
    {
      return mut() + mu();
    }
    //- Return the effective dynamic viscosity on patch
    virtual tmp<scalarField> muEff(const label patchi) const
    {
      return mut(patchi) + mu(patchi);
    }
    //- Return the phase-pressure'
    // (derivative of phase-pressure w.r.t. phase-fraction)
    virtual tmp<volScalarField> pPrime() const;
    //- Return the face-phase-pressure'
    // (derivative of phase-pressure w.r.t. phase-fraction)
    virtual tmp<surfaceScalarField> pPrimef() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "phase_compressible_turbulence_model.cpp"
#endif
#endif
