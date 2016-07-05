#ifndef TURBULENCE_MODELS_COMPRESSIBLE_EDDY_DIFFUSIVITY_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_EDDY_DIFFUSIVITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::EddyDiffusivity
// Description
//   Templated abstract base class for single-phase compressible
//   turbulence models.

namespace mousse {

template<class BasicTurbulenceModel>
class EddyDiffusivity
:
  public BasicTurbulenceModel
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Prt_;
    // Fields
      volScalarField alphat_;
  // Protected Member Functions
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  // Constructors
    //- Construct
    EddyDiffusivity
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
  //- Destructor
  virtual ~EddyDiffusivity()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read();
    //- Return the turbulent thermal diffusivity for enthalpy [kg/m/s]
    virtual tmp<volScalarField> alphat() const
    {
      return alphat_;
    }
    //- Return the turbulent thermal diffusivity for enthalpy for a patch
    //  [kg/m/s]
    virtual tmp<scalarField> alphat(const label patchi) const
    {
      return alphat()().boundaryField()[patchi];
    }
    //- Return the effective turbulent thermal diffusivity for temperature
    //  [J/m/s/K]
    virtual tmp<volScalarField> kappaEff() const
    {
      return this->transport_.kappaEff(alphat());
    }
    //- Return the effective turbulent thermal diffusivity for temperature
    //  [J/m/s/K]
    virtual tmp<scalarField> kappaEff(const label patchi) const
    {
      return this->transport_.kappaEff(alphat(patchi), patchi);
    }
    //- Return the effective turbulent thermal diffusivity for enthalpy
    //  [kg/m/s]
    virtual tmp<volScalarField> alphaEff() const
    {
      return this->transport_.alphaEff(alphat());
    }
    //- Return the effective turbulent thermal diffusivity for enthalpy
    //  for a patch [kg/m/s]
    virtual tmp<scalarField> alphaEff(const label patchi) const
    {
      return this->transport_.alphaEff(alphat(patchi), patchi);
    }
    //- Correct the turbulence thermal diffusivity for energy transport
    virtual void correctEnergyTransport();
};

}  // namespace mousse

#include "eddy_diffusivity.ipp"

#endif
