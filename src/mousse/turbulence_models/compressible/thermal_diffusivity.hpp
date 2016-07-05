#ifndef TURBULENCE_MODELS_COMPRESSIBLE_THERMAL_DIFFUSIVITY_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_THERMAL_DIFFUSIVITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ThermalDiffusivity
// Description
//   Templated wrapper class to provide compressible turbulence models
//   thermal diffusivity based thermal transport.

namespace mousse {

template<class BasicTurbulenceModel>
class ThermalDiffusivity
:
  public BasicTurbulenceModel
{
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef volScalarField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  // Constructors
    //- Construct
    ThermalDiffusivity
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
    static autoPtr<ThermalDiffusivity> New
    (
      const alphaField& alpha,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& trasportModel,
      const word& propertiesName = turbulenceModel::propertiesName
    );
    //- Return a reference to the selected turbulence model
    static autoPtr<ThermalDiffusivity> New
    (
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& phi,
      const transportModel& trasportModel,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~ThermalDiffusivity()
  {}
  // Member Functions
    //- Return the thermal diffusivity for temperature [J/m/s/K]
    virtual tmp<volScalarField> kappa() const
    {
      return this->transport_.kappa();
    }
    //- Return the laminar thermal diffusivity for temperature on patch
    //  [J/m/s/K]
    virtual tmp<scalarField> kappa(const label patchi) const
    {
      return this->transport_.kappa(patchi);
    }
    //- Return the laminar thermal diffusivity for enthalpy [kg/m/s]
    virtual tmp<volScalarField> alpha() const
    {
      return this->transport_.alpha();
    }
    //- Return the laminar thermal diffusivity for enthalpy on patch
    //  [kg/m/s]
    virtual tmp<scalarField> alpha(const label patchi) const
    {
      return this->transport_.alpha(patchi);
    }
    //- Return the turbulent thermal diffusivity for enthalpy [kg/m/s]
    virtual tmp<volScalarField> alphat() const;
    //- Return the turbulent thermal diffusivity for enthalpy for a patch
    //  [kg/m/s]
    virtual tmp<scalarField> alphat(const label patchi) const;
    //- Return the effective turbulent thermal diffusivity for temperature
    //  [J/m/s/K]
    virtual tmp<volScalarField> kappaEff() const
    {
      return kappa();
    }
    //- Return the effective turbulent thermal diffusivity for temperature
    //  [J/m/s/K]
    virtual tmp<scalarField> kappaEff(const label patchi) const
    {
      return kappa(patchi);
    }
    //- Return the effective turbulent thermal diffusivity for enthalpy
    //  [kg/m/s]
    virtual tmp<volScalarField> alphaEff() const
    {
      return alpha();
    }
    //- Return the effective turbulent thermal diffusivity for enthalpy
    //  for a patch [kg/m/s]
    virtual tmp<scalarField> alphaEff(const label patchi) const
    {
      return alpha(patchi);
    }
};

}  // namespace mousse

#include "thermal_diffusivity.ipp"

#endif
