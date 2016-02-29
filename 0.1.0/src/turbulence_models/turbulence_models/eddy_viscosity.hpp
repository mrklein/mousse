#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_EDDY_VISCOSITY_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_EDDY_VISCOSITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::eddyViscosity
// Group
//   grpTurbulence
// Description
//   Eddy viscosity turbulence model base class
// SourceFiles
//   eddy_viscosity.cpp
#include "linear_viscous_stress.hpp"
namespace mousse
{
template<class BasicTurbulenceModel>
class eddyViscosity
:
  public linearViscousStress<BasicTurbulenceModel>
{
protected:
  // Protected data
    // Fields
      volScalarField nut_;
  // Protected Member Functions
    virtual void correctNut() = 0;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  // Constructors
    //- Construct from components
    eddyViscosity
    (
      const word& modelName,
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName
    );
  //- Destructor
  virtual ~eddyViscosity()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read() = 0;
    //- Return non-const access to the turbulence viscosity
    //  to allow modification by means other than derivation
    volScalarField& evNut()
    {
      return nut_;
    }
    //- Return the turbulence viscosity
    virtual tmp<volScalarField> nut() const
    {
      return nut_;
    }
    //- Return the turbulence viscosity on patch
    virtual tmp<scalarField> nut(const label patchi) const
    {
      return nut_.boundaryField()[patchi];
    }
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const = 0;
    //- Return the Reynolds stress tensor
    virtual tmp<volSymmTensorField> R() const;
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct() = 0;
};
}  // namespace mousse
#ifdef NoRepository
#   include "eddy_viscosity.cpp"
#endif
#endif
