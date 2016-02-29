#ifndef TURBULENCE_MODELS_INCOMPRESSIBLE_TURBULENT_TRANSPORT_MODELS_RAS_LAM_BREMHORST_KE_HPP_
#define TURBULENCE_MODELS_INCOMPRESSIBLE_TURBULENT_TRANSPORT_MODELS_RAS_LAM_BREMHORST_KE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressible::RASModels::LamBremhorstKE
// Group
//   grpIcoRASTurbulence
// Description
//   Lam and Bremhorst low-Reynolds number k-epsilon turbulence model
//   for incompressible flows
//   This turbulence model is described in:
//   \verbatim
//     Lam, C. K. G., & Bremhorst, K. (1981).
//     A modified form of the k-ε model for predicting wall turbulence.
//     Journal of Fluids Engineering, 103(3), 456-460.
//   \endverbatim
// SourceFiles
//   lam_bremhorst_ke.cpp
#include "turbulent_transport_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace incompressible
{
namespace RASModels
{
class LamBremhorstKE
:
  public eddyViscosity<incompressible::RASModel>
{
  // Private Member Functions
    tmp<volScalarField> Rt() const;
    tmp<volScalarField> fMu(const volScalarField& Rt) const;
    tmp<volScalarField> f1(const volScalarField& fMu) const;
    tmp<volScalarField> f2(const volScalarField& Rt) const;
    void correctNut(const volScalarField& fMu);
protected:
  // Protected data
    dimensionedScalar Cmu_;
    dimensionedScalar Ceps1_;
    dimensionedScalar Ceps2_;
    dimensionedScalar sigmaEps_;
    volScalarField k_;
    volScalarField epsilon_;
    //- Wall distance
    //  Note: different to wall distance in parent RASModel
    //  which is for near-wall cells only
    const volScalarField& y_;
  // Protected Member Functions
    virtual void correctNut();
public:
  //- Runtime type information
  TYPE_NAME("LamBremhorstKE");
  // Constructors
    //- Construct from components
    LamBremhorstKE
    (
      const geometricOneField& alpha,
      const geometricOneField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
    // Disallow default bitwise copy construct and assignment
    LamBremhorstKE(const LamBremhorstKE&) = delete;
    LamBremhorstKE& operator=(const LamBremhorstKE&) = delete;
  //- Destructor
  virtual ~LamBremhorstKE()
  {}
  // Member Functions
    //- Read RASProperties dictionary
    virtual bool read();
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField("DkEff", nut_ + nu())
      );
    }
    //- Return the effective diffusivity for epsilon
    tmp<volScalarField> DepsilonEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField("DepsilonEff", nut_/sigmaEps_ + nu())
      );
    }
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k_;
    }
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const
    {
      return epsilon_;
    }
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace incompressible
}  // namespace mousse
#endif
