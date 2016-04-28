#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TCOMPRESSIBLE_TURBULENCE_MODEL_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TCOMPRESSIBLE_TURBULENCE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CompressibleTurbulenceModel
// Description
//   Templated abstract base class for single-phase compressible
//   turbulence models.

#include "_turbulence_model.hpp"
#include "compressible_turbulence_model.hpp"


namespace mousse {

template<class TransportModel>
class CompressibleTurbulenceModel
:
  public TurbulenceModel
  <
    geometricOneField,
    volScalarField,
    compressibleTurbulenceModel,
    TransportModel
  >
{
public:
  typedef geometricOneField alphaField;
  typedef volScalarField rhoField;
  typedef TransportModel transportModel;
  // Constructors
    //- Construct
    CompressibleTurbulenceModel
    (
      const word& type,
      const geometricOneField& alpha,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& trasport,
      const word& propertiesName
    );
  // Selectors
    //- Return a reference to the selected turbulence model
    static autoPtr<CompressibleTurbulenceModel> New
    (
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& phi,
      const transportModel& trasportModel,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~CompressibleTurbulenceModel()
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
};

}  // namespace mousse

#include "_compressible_turbulence_model.ipp"

#endif
