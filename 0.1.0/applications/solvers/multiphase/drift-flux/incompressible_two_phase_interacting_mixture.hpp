// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressibleTwoPhaseInteractingMixture
// Description
//   A two-phase incompressible transportModel for interacting phases
//   requiring the direct evaluation of the mixture viscosity,
//   e.g. activated sludge or slurry.
// SourceFiles
//   incompressible_two_phase_interacting_mixture.cpp
#ifndef INCOMPRESSIBLE_TWO_PHASE_INTERACTING_MIXTURE_HPP_
#define INCOMPRESSIBLE_TWO_PHASE_INTERACTING_MIXTURE_HPP_
#include "compressible_transport_model.hpp"
#include "incompressible/viscosity_model.hpp"
#include "mixture_viscosity_model.hpp"
#include "two_phase_mixture.hpp"
#include "iodictionary.hpp"
namespace mousse
{
class incompressibleTwoPhaseInteractingMixture
:
  public IOdictionary,
  public compressibleTransportModel,
  public twoPhaseMixture
{
protected:
  // Protected data
    autoPtr<mixtureViscosityModel> muModel_;
    autoPtr<viscosityModel> nucModel_;
    dimensionedScalar rhod_;
    dimensionedScalar rhoc_;
    //- Optional diameter of the dispersed phase particles
    dimensionedScalar dd_;
    //- Optional maximum dispersed phase-fraction (e.g. packing limit)
    scalar alphaMax_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;
    volScalarField mu_;
public:
  TYPE_NAME("incompressibleTwoPhaseInteractingMixture");
  // Constructors
    //- Construct from components
    incompressibleTwoPhaseInteractingMixture
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~incompressibleTwoPhaseInteractingMixture()
  {}
  // Member Functions
    //- Return const-access to the mixture viscosityModel
    const mixtureViscosityModel& muModel() const
    {
      return muModel_();
    }
    //- Return const-access to the continuous-phase viscosityModel
    const viscosityModel& nucModel() const
    {
      return nucModel_();
    }
    //- Return const-access to the dispersed-phase density
    const dimensionedScalar& rhod() const
    {
      return rhod_;
    }
    //- Return const-access to continuous-phase density
    const dimensionedScalar& rhoc() const
    {
      return rhoc_;
    };
    //- Return the diameter of the dispersed-phase particles
    const dimensionedScalar& dd() const
    {
      return dd_;
    }
    //- Optional maximum phase-fraction (e.g. packing limit)
    //  Defaults to 1
    scalar alphaMax() const
    {
      return alphaMax_;
    }
    //- Return const-access to the mixture velocity
    const volVectorField& U() const
    {
      return U_;
    }
    //- Return the dynamic mixture viscosity
    tmp<volScalarField> mu() const
    {
      return mu_;
    }
    //- Return the dynamic mixture viscosity for patch
    virtual tmp<scalarField> mu(const label patchi) const
    {
      return mu_.boundaryField()[patchi];
    }
    //- Return the mixture density
    virtual tmp<volScalarField> rho() const
    {
      return alpha1_*rhod_ + alpha2_*rhoc_;
    }
    //- Return the mixture density for patch
    virtual tmp<scalarField> rho(const label patchi) const
    {
      return
        alpha1_.boundaryField()[patchi]*rhod_.value()
        + alpha2_.boundaryField()[patchi]*rhoc_.value();
    }
    //- Return the mixture viscosity
    virtual tmp<volScalarField> nu() const
    {
      return mu_/rho();
    }
    //- Return the mixture viscosity for patch
    virtual tmp<scalarField> nu(const label patchi) const
    {
      return mu_.boundaryField()[patchi]/rho(patchi);
    }
    //- Correct the laminar viscosity
    virtual void correct()
    {
      mu_ = muModel_->mu(rhoc_*nucModel_->nu());
    }
    //- Read base transportProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
