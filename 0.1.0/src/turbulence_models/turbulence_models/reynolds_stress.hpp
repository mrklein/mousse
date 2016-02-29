#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_REYNOLDS_STRESS_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_REYNOLDS_STRESS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReynoldsStress
// Group
//   grpTurbulence
// Description
//   Reynolds-stress turbulence model base class
// SourceFiles
//   reynolds_stress.cpp
namespace mousse
{
template<class BasicTurbulenceModel>
class ReynoldsStress
:
  public BasicTurbulenceModel
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar couplingFactor_;
    // Fields
      volSymmTensorField R_;
      volScalarField nut_;
  // Protected Member Functions
    void boundNormalStress(volSymmTensorField& R) const;
    void correctWallShearStress(volSymmTensorField& R) const;
    //- Update the eddy-viscosity
    virtual void correctNut() = 0;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  // Constructors
    //- Construct from components
    ReynoldsStress
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
  virtual ~ReynoldsStress()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read() = 0;
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
    virtual tmp<volScalarField> k() const;
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
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct() = 0;
};
}  // namespace mousse
#ifdef NoRepository
#   include "reynolds_stress.cpp"
#endif
#endif
