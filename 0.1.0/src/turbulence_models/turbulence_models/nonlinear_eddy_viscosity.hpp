#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_NONLINEAR_EDDY_VISCOSITY_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_NONLINEAR_EDDY_VISCOSITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonlinearEddyViscosity
// Group
//   grpTurbulence
// Description
//   Eddy viscosity turbulence model with non-linear correction base class
// SourceFiles
//   nonlinear_eddy_viscosity.cpp
#include "eddy_viscosity.hpp"
namespace mousse
{
template<class BasicTurbulenceModel>
class nonlinearEddyViscosity
:
  public eddyViscosity<BasicTurbulenceModel>
{
protected:
  // Protected data
    // Fields
      volSymmTensorField nonlinearStress_;
  // Protected Member Functions
    virtual void correctNonlinearStress(const volTensorField& gradU) = 0;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  // Constructors
    //- Construct from components
    nonlinearEddyViscosity
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
  virtual ~nonlinearEddyViscosity()
  {}
  // Member Functions
    //- Return the Reynolds stress tensor
    virtual tmp<volSymmTensorField> R() const;
    //- Return the effective stress tensor
    virtual tmp<volSymmTensorField> devRhoReff() const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff
    (
      const volScalarField& rho,
      volVectorField& U
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "nonlinear_eddy_viscosity.cpp"
#endif
#endif
