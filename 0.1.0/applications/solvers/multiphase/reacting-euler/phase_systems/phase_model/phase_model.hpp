// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseModel
// SourceFiles
//   phase_model.cpp
#ifndef PHASE_MODEL_HPP_
#define PHASE_MODEL_HPP_
#include "dictionary.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrices_fwd.hpp"
#include "rho_thermo.hpp"
#include "euler_phase_compressible_turbulence_model_fwd.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phaseSystem;
class diameterModel;
class phaseModel
:
  public volScalarField
{
  // Private data
    //- Reference to the phaseSystem to which this phase belongs
    const phaseSystem& fluid_;
    //- Name of phase
    word name_;
    //- Index of phase
    label index_;
    //- Return the residual phase-fraction for given phase
    //  Used to stabilize the phase momentum as the phase-fraction -> 0
    dimensionedScalar residualAlpha_;
    //- Optional maximum phase-fraction (e.g. packing limit)
    scalar alphaMax_;
    //- Diameter model
    autoPtr<diameterModel> diameterModel_;
public:
  //- Runtime type information
  CLASS_NAME("phaseModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      phaseModel,
      phaseSystem,
      (
        const phaseSystem& fluid,
        const word& phaseName,
        const label index
      ),
      (fluid, phaseName, index)
    );
  // Constructors
    phaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
    //- Return clone
    autoPtr<phaseModel> clone() const;
  // Selectors
    static autoPtr<phaseModel> New
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
    //- Return a pointer to a new phase created on freestore
    //  from Istream
    class iNew
    {
      const phaseSystem& fluid_;
      mutable label indexCounter_;
    public:
      iNew
      (
        const phaseSystem& fluid
      )
      :
        fluid_(fluid),
        indexCounter_(-1)
      {}
      autoPtr<phaseModel> operator()(Istream& is) const
      {
        indexCounter_++;
        return autoPtr<phaseModel>
        {
          phaseModel::New(fluid_, word(is), indexCounter_)
        };
      }
    };
  //- Destructor
  virtual ~phaseModel();
  // Member Functions
    //- Return the name of this phase
    const word& name() const;
    //- Return the name of the phase for use as the keyword in PtrDictionary
    const word& keyword() const;
    //- Return the index of the phase
    label index() const;
    //- Return the system to which this phase belongs
    const phaseSystem& fluid() const;
    //- Return the residual phase-fraction for given phase
    //  Used to stabilize the phase momentum as the phase-fraction -> 0
    const dimensionedScalar& residualAlpha() const;
    //- Return the maximum phase-fraction (e.g. packing limit)
    scalar alphaMax() const;
    //- Return the Sauter-mean diameter
    tmp<volScalarField> d() const;
    //- Correct the phase properties
    virtual void correct();
    //- Correct the kinematics
    virtual void correctKinematics();
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Correct the turbulence
    virtual void correctTurbulence();
    //- Correct the energy transport e.g. alphat
    virtual void correctEnergyTransport();
    //- Return the momentum equation
    virtual tmp<fvVectorMatrix> UEqn() = 0;
    //- Return the enthalpy equation
    virtual tmp<fvScalarMatrix> heEqn() = 0;
    //- Return the species fraction equation
    virtual tmp<fvScalarMatrix> YiEqn(volScalarField& Yi) = 0;
    //- Read phase properties dictionary
    virtual bool read();
    // Compressibility (variable density)
      //- Return true if the phase is compressible otherwise false
      virtual bool compressible() const;
      //- Return the phase dilatation rate (d(alpha)/dt + div(alpha*phi))
      virtual const tmp<volScalarField>& divU() const;
      //- Set the phase dilatation rate (d(alpha)/dt + div(alpha*phi))
      virtual void divU(const tmp<volScalarField>& divU);
      //- Return the phase kinetic energy
      virtual const volScalarField& K() const;
    // Implicit phase pressure and dispersion support
      //- Return the phase diffusivity divided by the momentum coefficient
      virtual const surfaceScalarField& DbyA() const;
      //- Set the phase diffusivity divided by the momentum coefficient
      virtual void DbyA(const tmp<surfaceScalarField>& DbyA);
    // Thermo
      //- Return const access to the thermophysical model
      virtual const rhoThermo& thermo() const = 0;
      //- Return non-const access to the thermophysical model
      //  for correction
      virtual rhoThermo& thermo() = 0;
      //- Return the density field
      virtual tmp<volScalarField> rho() const = 0;
      //- Constant access the species mass fractions
      virtual const PtrList<volScalarField>& Y() const = 0;
      //- Access the species mass fractions
      virtual PtrList<volScalarField>& Y() = 0;
    // Momentum
      //- Constant access the velocity
      virtual tmp<volVectorField> U() const = 0;
      //- Access the velocity
      virtual volVectorField& U() = 0;
      //- Return the substantive acceleration
      virtual tmp<volVectorField> DUDt() const = 0;
      //- Constant access the continuity error
      virtual tmp<volScalarField> continuityError() const = 0;
      //- Constant access the volumetric flux
      virtual tmp<surfaceScalarField> phi() const = 0;
      //- Access the volumetric flux
      virtual surfaceScalarField& phi() = 0;
      //- Constant access the volumetric flux of the phase
      virtual tmp<surfaceScalarField> alphaPhi() const = 0;
      //- Access the volumetric flux of the phase
      virtual surfaceScalarField& alphaPhi() = 0;
      //- Constant access the mass flux of the phase
      virtual tmp<surfaceScalarField> alphaRhoPhi() const = 0;
      //- Access the mass flux of the phase
      virtual surfaceScalarField& alphaRhoPhi() = 0;
    // Transport
      //- Return the laminar dynamic viscosity
      virtual tmp<volScalarField> mu() const = 0;
      //- Return the laminar dynamic viscosity on a patch
      virtual tmp<scalarField> mu(const label patchi) const = 0;
      //- Return the laminar kinematic viscosity
      virtual tmp<volScalarField> nu() const = 0;
      //- Return the laminar kinematic viscosity on a patch
      virtual tmp<scalarField> nu(const label patchi) const = 0;
      //- Return the laminar thermal conductivity
      virtual tmp<volScalarField> kappa() const = 0;
      //- Return the laminar thermal conductivity on a patch
      virtual tmp<scalarField> kappa(const label patchi) const = 0;
      //- Return the effective thermal conductivity
      virtual tmp<volScalarField> kappaEff
      (
        const volScalarField& alphat
      ) const = 0;
      //- Access the effective thermal conductivity
      virtual tmp<scalarField> kappaEff
      (
        const scalarField& alphat,
        const label patchi
      ) const = 0;
      //- Return the laminar thermal diffusivity for enthalpy
      virtual tmp<volScalarField> alpha() const = 0;
      //- Return the laminar thermal diffusivity for enthalpy on a patch
      virtual tmp<scalarField> alpha(const label patchi) const = 0;
      //- Return the effective thermal diffusivity for enthalpy
      virtual tmp<volScalarField> alphaEff
      (
        const volScalarField& alphat
      ) const = 0;
      //- Return the effective thermal diffusivity for enthalpy on a patch
      virtual tmp<scalarField> alphaEff
      (
        const scalarField& alphat,
        const label patchi
      ) const = 0;
    // Turbulence
      //- Return the turbulence model
      virtual const phaseCompressibleTurbulenceModel&
        turbulence() const = 0;
};
}  // namespace mousse
#endif
