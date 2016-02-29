#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_DYNAMIC_LAGRANGIAN_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_DYNAMIC_LAGRANGIAN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::dynamicLagrangian
// Group
//   grpLESTurbulence
// Description
//   Dynamic SGS model with Lagrangian averaging
//   Reference:
//   \verbatim
//     Meneveau, C., Lund, T. S., & Cabot, W. H. (1996).
//     A Lagrangian dynamic subgrid-scale model of turbulence.
//     Journal of Fluid Mechanics, 319, 353-385.
//   \endverbatim
// SourceFiles
//   dynamic_lagrangian.cpp
#include "les_eddy_viscosity.hpp"
#include "simple_filter.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class dynamicLagrangian
:
  public LESeddyViscosity<BasicTurbulenceModel>
{
protected:
  // Protected data
    volScalarField flm_;
    volScalarField fmm_;
    dimensionedScalar theta_;
    simpleFilter simpleFilter_;
    autoPtr<LESfilter> filterPtr_;
    LESfilter& filter_;
    dimensionedScalar flm0_;
    dimensionedScalar fmm0_;
  // Protected Member Functions
    //- Update sub-grid eddy-viscosity
    void correctNut(const tmp<volTensorField>& gradU);
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("dynamicLagrangian");
  // Constructors
    //- Construct from components
    dynamicLagrangian
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
    // Disallow default bitwise copy construct and assignment
    dynamicLagrangian(const dynamicLagrangian&) = delete;
    dynamicLagrangian& operator=(const dynamicLagrangian&) = delete;
  //- Destructor
  virtual ~dynamicLagrangian()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return SGS kinetic energy
    tmp<volScalarField> k(const tmp<volTensorField>& gradU) const
    {
      return
        pow(2.0*flm_/fmm_, 2.0/3.0)
       * pow(this->Ce_, -2.0/3.0)
       * sqr(this->delta())*magSqr(dev(symm(gradU)));
    }
    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k(fvc::grad(this->U_));
    }
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField("DkEff", this->nut_ + this->nu())
      );
    }
    //- Correct Eddy-Viscosity and related properties
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "dynamic_lagrangian.cpp"
#endif
#endif
