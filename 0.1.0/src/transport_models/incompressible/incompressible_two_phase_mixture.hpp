// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressibleTwoPhaseMixture
// Description
//   A two-phase incompressible transportModel
// SourceFiles
//   incompressible_two_phase_mixture.cpp
#ifndef incompressible_two_phase_mixture_hpp_
#define incompressible_two_phase_mixture_hpp_
#include "incompressible/transport_model.hpp"
#include "incompressible/viscosity_model.hpp"
#include "two_phase_mixture.hpp"
#include "iodictionary.hpp"
namespace mousse
{
class incompressibleTwoPhaseMixture
:
  public IOdictionary,
  public transportModel,
  public twoPhaseMixture
{
protected:
  // Protected data
    autoPtr<viscosityModel> nuModel1_;
    autoPtr<viscosityModel> nuModel2_;
    dimensionedScalar rho1_;
    dimensionedScalar rho2_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;
    volScalarField nu_;
  // Private Member Functions
    //- Calculate and return the laminar viscosity
    void calcNu();
public:
  TypeName("incompressibleTwoPhaseMixture");
  // Constructors
    //- Construct from components
    incompressibleTwoPhaseMixture
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~incompressibleTwoPhaseMixture()
  {}
  // Member Functions
    //- Return const-access to phase1 viscosityModel
    const viscosityModel& nuModel1() const
    {
      return nuModel1_();
    }
    //- Return const-access to phase2 viscosityModel
    const viscosityModel& nuModel2() const
    {
      return nuModel2_();
    }
    //- Return const-access to phase1 density
    const dimensionedScalar& rho1() const
    {
      return rho1_;
    }
    //- Return const-access to phase2 density
    const dimensionedScalar& rho2() const
    {
      return rho2_;
    };
    //- Return const-access to the mixture velocity
    const volVectorField& U() const
    {
      return U_;
    }
    //- Return the dynamic laminar viscosity
    tmp<volScalarField> mu() const;
    //- Return the face-interpolated dynamic laminar viscosity
    tmp<surfaceScalarField> muf() const;
    //- Return the kinematic laminar viscosity
    virtual tmp<volScalarField> nu() const
    {
      return nu_;
    }
    //- Return the laminar viscosity for patch
    virtual tmp<scalarField> nu(const label patchi) const
    {
      return nu_.boundaryField()[patchi];
    }
    //- Return the face-interpolated kinematic laminar viscosity
    tmp<surfaceScalarField> nuf() const;
    //- Correct the laminar viscosity
    virtual void correct()
    {
      calcNu();
    }
    //- Read base transportProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
