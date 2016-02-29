// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressibleThreePhaseMixture
// Description
// SourceFiles
//   incompressible_three_phase_mixture.cpp
#ifndef INCOMPRESSIBLE_THREE_PHASE_MIXTURE_HPP_
#define INCOMPRESSIBLE_THREE_PHASE_MIXTURE_HPP_
#include "incompressible/transport_model.hpp"
#include "iodictionary.hpp"
#include "incompressible/viscosity_model.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class incompressibleThreePhaseMixture
:
  public IOdictionary,
  public transportModel
{
  // Private data
    word phase1Name_;
    word phase2Name_;
    word phase3Name_;
    volScalarField alpha1_;
    volScalarField alpha2_;
    volScalarField alpha3_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;
    volScalarField nu_;
    autoPtr<viscosityModel> nuModel1_;
    autoPtr<viscosityModel> nuModel2_;
    autoPtr<viscosityModel> nuModel3_;
    dimensionedScalar rho1_;
    dimensionedScalar rho2_;
    dimensionedScalar rho3_;
  // Private Member Functions
    //- Calculate and return the laminar viscosity
    void calcNu();
public:
  // Constructors
    //- Construct from components
    incompressibleThreePhaseMixture
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  ~incompressibleThreePhaseMixture()
  {}
  // Member Functions
    const word phase1Name() const
    {
      return phase1Name_;
    }
    const word phase2Name() const
    {
      return phase2Name_;
    }
    const word phase3Name() const
    {
      return phase3Name_;
    }
    const volScalarField& alpha1() const
    {
      return alpha1_;
    }
    volScalarField& alpha1()
    {
      return alpha1_;
    }
    const volScalarField& alpha2() const
    {
      return alpha2_;
    }
    volScalarField& alpha2()
    {
      return alpha2_;
    }
    const volScalarField& alpha3() const
    {
      return alpha3_;
    }
    volScalarField& alpha3()
    {
      return alpha3_;
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
    //- Return const-access to phase3 density
    const dimensionedScalar& rho3() const
    {
      return rho3_;
    };
    //- Return the velocity
    const volVectorField& U() const
    {
      return U_;
    }
    //- Return the flux
    const surfaceScalarField& phi() const
    {
      return phi_;
    }
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
    //- Return const-access to phase3 viscosityModel
    const viscosityModel& nuModel3() const
    {
      return nuModel3_();
    }
    //- Return the dynamic laminar viscosity
    tmp<volScalarField> mu() const;
    //- Return the face-interpolated dynamic laminar viscosity
    tmp<surfaceScalarField> muf() const;
    //- Return the kinematic laminar viscosity
    tmp<volScalarField> nu() const
    {
      return nu_;
    }
    //- Return the laminar viscosity for patch
    tmp<scalarField> nu(const label patchi) const
    {
      return nu_.boundaryField()[patchi];
    }
    //- Return the face-interpolated dynamic laminar viscosity
    tmp<surfaceScalarField> nuf() const;
    //- Correct the laminar viscosity
    void correct()
    {
      calcNu();
    }
    //- Read base transportProperties dictionary
    bool read();
};
}  // namespace mousse
#endif
