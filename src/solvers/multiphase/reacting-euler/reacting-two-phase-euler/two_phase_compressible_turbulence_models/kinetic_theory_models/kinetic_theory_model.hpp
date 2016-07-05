#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_KINETIC_THEORY_MODEL_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_KINETIC_THEORY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModel
// Description
//   Kinetic theory particle phase RAS model
//   Reference:
//   \verbatim
//     "Derivation, implementation, and validation of computer simulation
//     models for gas-solid fluidized beds",
//     van Wachem, B.G.M.,
//     Ph.D. Thesis, Delft University of Technology, Amsterdam, 2000.
//   \endverbatim
//   There are no default model coefficients.
// SourceFiles
//   kinetic_theory_model.cpp
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
#include "euler_phase_compressible_turbulence_model.hpp"
#include "eddy_diffusivity.hpp"
#include "phase_model.hpp"
#include "drag_model.hpp"
#include "viscosity_model.hpp"
#include "conductivity_model.hpp"
#include "radial_model.hpp"
#include "granular_pressure_model.hpp"
#include "frictional_stress_model.hpp"
namespace mousse
{
namespace RASModels
{
class kineticTheoryModel
:
  public eddyViscosity
  <
    RASModel<EddyDiffusivity<phaseCompressibleTurbulenceModel>>
  >
{
  // Private data
    // Input Fields
      const phaseModel& phase_;
    // Sub-models
      //- Run-time selected viscosity model
      autoPtr<kineticTheoryModels::viscosityModel> viscosityModel_;
      //- Run-time selected conductivity model
      autoPtr<kineticTheoryModels::conductivityModel> conductivityModel_;
      //- Run-time selected radial distribution model
      autoPtr<kineticTheoryModels::radialModel> radialModel_;
      //- Run-time selected granular pressure model
      autoPtr<kineticTheoryModels::granularPressureModel>
        granularPressureModel_;
      //- Run-time selected frictional stress model
      autoPtr<kineticTheoryModels::frictionalStressModel>
        frictionalStressModel_;
    // Kinetic Theory Model coefficients
      //- Use equilibrium approximation: generation == dissipation
      Switch equilibrium_;
      //- Coefficient of restitution
      dimensionedScalar e_;
      //- Maximum packing phase-fraction
      dimensionedScalar alphaMax_;
      //- Min value for which the frictional stresses are zero
      dimensionedScalar alphaMinFriction_;
      //- Residual phase fraction
      dimensionedScalar residualAlpha_;
    // Kinetic Theory Model Fields
      //- The granular energy/temperature
      volScalarField Theta_;
      //- The granular bulk viscosity
      volScalarField lambda_;
      //- The granular radial distribution
      volScalarField gs0_;
      //- The granular "thermal" conductivity
      volScalarField kappa_;
  // Private Member Functions
    void correctNut()
    {}
public:
  //- Runtime type information
  TYPE_NAME("kineticTheory");
  // Constructors
    //- Construct from components
    kineticTheoryModel
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
    kineticTheoryModel(const kineticTheoryModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const kineticTheoryModel&) = delete;
  //- Destructor
  virtual ~kineticTheoryModel();
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
