// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::v2f
// Group
//   grpRASTurbulence
// Description
//   Lien and Kalitzin's v2-f turbulence model for incompressible and
//   compressible flows, with a limit imposed on the turbulent viscosity given
//   by Davidson et al.
//   The model solves for turbulence kinetic energy k and turbulence dissipation
//   rate epsilon, with additional equations for the turbulence stress normal to
//   streamlines, v2, and elliptic damping function, f.
//   The variant implemented employs N=6, such that f=0 on walls.
//   Wall boundary conditions are:
//     k       = kLowReWallFunction
//     epsilon = epsilonLowReWallFunction
//     v2      = v2WallFunction
//     f       = fWallFunction
//   These are applicable to both low- and high-Reynolds number flows.
//   Inlet values can be approximated by:
//     v2      = 2/3 k
//     f       = zero-gradient
//   References:
//   \verbatim
//     Lien, F. S., & Kalitzin, G. (2001).
//     Computations of transonic flow with the v2f turbulence model.
//     International Journal of Heat and Fluid Flow, 22(1), 53-61.
//     Davidson, L., Nielsen, P., & Sveningsson, A. (2003).
//     Modifications of the v2-f model for computing the flow in a
//     3D wall jet.
//     Turbulence, Heat and Mass Transfer, 4, 577-584
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     v2fCoeffs
//     {
//       Cmu         0.22;
//       CmuKEps     0.09;
//       C1          1.4;
//       C2          0.3;
//       CL          0.23;
//       Ceta        70;
//       Ceps2       1.9;
//       Ceps3       -0.33;
//       sigmaEps    1.3;
//       sigmaK      1;
//     }
//   \endverbatim
// Note
//   If the kLowReWallFunction is employed, a velocity variant of the turbulent
//   viscosity wall function should be used, e.g. nutUWallFunction.  Turbulence
//   k variants (nutk...) for this case will not behave correctly.
// SeeAlso
//   mousse::RASModels::v2fBase
//   mousse::RASModels::kEpsilon
//   mousse::kLowReWallFunctionFvPatchScalarField
//   mousse::epsilonLowReWallFunctionFvPatchScalarField
//   mousse::v2WallFunctionFvPatchScalarField
//   mousse::fWallFunctionFvPatchScalarField
// SourceFiles
//   v2f.cpp
#ifndef v2f_hpp_
#define v2f_hpp_
#include "v2f_base.hpp"
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class v2f
:
  public eddyViscosity<RASModel<BasicTurbulenceModel>>,
  public v2fBase
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar CmuKEps_;
      dimensionedScalar C1_;
      dimensionedScalar C2_;
      dimensionedScalar CL_;
      dimensionedScalar Ceta_;
      dimensionedScalar Ceps2_;
      dimensionedScalar Ceps3_;
      dimensionedScalar sigmaK_;
      dimensionedScalar sigmaEps_;
    // Fields
      //- Turbulence kinetic energy
      volScalarField k_;
      //- Turbulence dissipation
      volScalarField epsilon_;
      //- Turbulence stress normal to streamlines
      volScalarField v2_;
      //- Damping function
      volScalarField f_;
    // Bounding values
      dimensionedScalar v2Min_;
      dimensionedScalar fMin_;
  // Protected Member Functions
    virtual void correctNut();
    //- Return time scale, Ts
    tmp<volScalarField> Ts() const;
    //- Return length scale, Ls
    tmp<volScalarField> Ls() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("v2f");
  // Constructors
    //- Construct from components
    v2f
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
  //- Destructor
  virtual ~v2f()
  {}
  // Member Functions
    //- Read RASProperties dictionary
    virtual bool read();
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DkEff",
          this->nut_/sigmaK_ + this->nu()
        )
      );
    }
    //- Return the effective diffusivity for epsilon
    tmp<volScalarField> DepsilonEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DepsilonEff",
          this->nut_/sigmaEps_ + this->nu()
        )
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
    //- Return turbulence stress normal to streamlines
    virtual tmp<volScalarField> v2() const
    {
      return v2_;
    }
    //- Return the damping function
    virtual tmp<volScalarField> f() const
    {
      return f_;
    }
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace mousse
#ifdef NoRepository
#   include "v2f.cpp"
#endif
#endif
