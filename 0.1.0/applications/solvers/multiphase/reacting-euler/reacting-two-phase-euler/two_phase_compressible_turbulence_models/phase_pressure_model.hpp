#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_PHASE_PRESSURE_MODEL_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_PHASE_PRESSURE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phasePressureModel
// Description
//   Particle-particle phase-pressure RAS model
//   The derivative of the phase-pressure with respect to the phase-fraction
//   is evaluated as
//     g0*min(exp(preAlphaExp*(alpha - alphaMax)), expMax)
//   The default model coefficients correspond to the following:
//   \verbatim
//     phasePressureCoeffs
//     {
//       preAlphaExp     500;
//       expMax          1000;
//       alphaMax        0.62;
//       g0              1000;
//     }
//   \endverbatim
// SourceFiles
//   phase_pressure_model.cpp
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
#include "euler_phase_compressible_turbulence_model.hpp"
#include "thermal_diffusivity.hpp"
#include "eddy_diffusivity.hpp"
#include "phase_model.hpp"
namespace mousse
{
namespace RASModels
{
class phasePressureModel
:
  public eddyViscosity
  <
    RASModel<EddyDiffusivity<ThermalDiffusivity
    <
      PhaseCompressibleTurbulenceModel<phaseModel>
    >>>
  >
{
  // Private data
    // Input Fields
    //  const phaseModel& phase_;
    // Kinetic Theory Model coefficients
      //- Maximum packing phase-fraction
      scalar alphaMax_;
      //- Pre-exponential factor
      scalar preAlphaExp_;
      //- Maximum limit of the exponential
      scalar expMax_;
      //- g0
      dimensionedScalar g0_;
  // Private Member Functions
    void correctNut()
    {}
public:
  //- Runtime type information
  TYPE_NAME("phasePressure");
  // Constructors
    //- Construct from components
    phasePressureModel
    (
      const volScalarField& alpha,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const phaseModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
    //- Disallow default bitwise copy construct
    phasePressureModel(const phasePressureModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const phasePressureModel&) = delete;
  //- Destructor
  virtual ~phasePressureModel();
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read();
    //- Return the effective viscosity
    virtual tmp<volScalarField> nuEff() const
    {
      return this->nut();
    }
    //- Return the effective viscosity on patch
    virtual tmp<scalarField> nuEff(const label patchi) const
    {
      return this->nut(patchi);
    }
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const;
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const;
    //- Return the Reynolds stress tensor
    virtual tmp<volSymmTensorField> R() const;
    //- Return the phase-pressure'
    // (derivative of phase-pressure w.r.t. phase-fraction)
    virtual tmp<volScalarField> pPrime() const;
    //- Return the face-phase-pressure'
    // (derivative of phase-pressure w.r.t. phase-fraction)
    virtual tmp<surfaceScalarField> pPrimef() const;
    //- Return the effective stress tensor
    virtual tmp<volSymmTensorField> devRhoReff() const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const;
    //- Solve the kinetic theory equations and correct the viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace mousse
#endif
