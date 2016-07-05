#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_SSG_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_SSG_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::SSG
// Group
//   grpRASTurbulence
// Description
//   Speziale, Sarkar and Gatski Reynolds-stress turbulence model for
//   incompressible and compressible flows.
//   Reference:
//   \verbatim
//     Speziale, C. G., Sarkar, S., & Gatski, T. B. (1991).
//     Modelling the pressure–strain correlation of turbulence:
//     an invariant dynamical systems approach.
//     Journal of Fluid Mechanics, 227, 245-272.
//   \endverbatim
//   Including the generalized gradient diffusion model of
//   Daly and Harlow:
//   \verbatim
//     Daly, B. J., & Harlow, F. H. (1970).
//     Transport equations in turbulence.
//     Physics of Fluids (1958-1988), 13(11), 2634-2649.
//   \endverbatim
//   The default model coefficients are:
//   \verbatim
//     SSGCoeffs
//     {
//       Cmu             0.09;
//       C1              3.4;
//       C1s             1.8;
//       C2              4.2;
//       C3              0.8;
//       C3s             1.3;
//       C4              1.25;
//       C5              0.4;
//       Ceps1           1.44;
//       Ceps2           1.92;
//       Cs              0.25;
//       Ceps            0.15;
//       couplingFactor  0.0;
//     }
//   \endverbatim

#include "ras_model.hpp"
#include "reynolds_stress.hpp"


namespace mousse {
namespace RASModels {

template<class BasicTurbulenceModel>
class SSG
:
  public ReynoldsStress<RASModel<BasicTurbulenceModel>>
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar C1_;
      dimensionedScalar C1s_;
      dimensionedScalar C2_;
      dimensionedScalar C3_;
      dimensionedScalar C3s_;
      dimensionedScalar C4_;
      dimensionedScalar C5_;
      dimensionedScalar Ceps1_;
      dimensionedScalar Ceps2_;
      dimensionedScalar Cs_;
      dimensionedScalar Ceps_;
    // Fields
      volScalarField k_;
      volScalarField epsilon_;
  // Protected Member Functions
    //- Update the eddy-viscosity
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("SSG");
  // Constructors
    //- Construct from components
    SSG
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
    SSG(const SSG&) = delete;
    SSG& operator=(const SSG&) = delete;
  //- Destructor
  virtual ~SSG()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
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
    //- Return the effective diffusivity for R
    tmp<volSymmTensorField> DREff() const;
    //- Return the effective diffusivity for epsilon
    tmp<volSymmTensorField> DepsilonEff() const;
    //- Solve the turbulence equations and correct eddy-Viscosity and
    //  related properties
    virtual void correct();
};

}  // namespace RASModels
}  // namespace mousse

#include "ssg.ipp"

#endif
