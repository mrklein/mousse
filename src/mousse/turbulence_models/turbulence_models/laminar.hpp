#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LAMINAR_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LAMINAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminar
// Description
//   Turbulence model for laminar flow.

#include "linear_viscous_stress.hpp"


namespace mousse {

template<class BasicTurbulenceModel>
class laminar
:
  public linearViscousStress<BasicTurbulenceModel>
{
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("laminar");
  // Constructors
    //- Construct from components
    laminar
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  // Selectors
    //- Return a reference to the selected turbulence model
    static autoPtr<laminar> New
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~laminar()
  {}
  // Member Functions
    //- Const access to the coefficients dictionary
    virtual const dictionary& coeffDict() const;
    //- Read turbulenceProperties dictionary
    virtual bool read();
    //- Return the turbulence viscosity, i.e. 0 for laminar flow
    virtual tmp<volScalarField> nut() const;
    //- Return the turbulence viscosity on patch
    virtual tmp<scalarField> nut(const label patchi) const;
    //- Return the effective viscosity, i.e. the laminar viscosity
    virtual tmp<volScalarField> nuEff() const;
    //- Return the effective viscosity on patch
    virtual tmp<scalarField> nuEff(const label patchi) const;
    //- Return the turbulence kinetic energy, i.e. 0 for laminar flow
    virtual tmp<volScalarField> k() const;
    //- Return the turbulence kinetic energy dissipation rate,
    //  i.e. 0 for laminar flow
    virtual tmp<volScalarField> epsilon() const;
    //- Return the Reynolds stress tensor, i.e. 0 for laminar flow
    virtual tmp<volSymmTensorField> R() const;
    //- Correct the laminar viscosity
    virtual void correct();
};

}  // namespace mousse

#include "laminar.ipp"

#endif
