// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::LRR
// Group
//   grpRASTurbulence
// Description
//   Launder, Reece and Rodi Reynolds-stress turbulence model for
//   incompressible and compressible flows.
//   Reference:
//   \verbatim
//     Launder, B. E., Reece, G. J., & Rodi, W. (1975).
//     Progress in the development of a Reynolds-stress turbulence closure.
//     Journal of fluid mechanics, 68(03), 537-566.
//   \endverbatim
//   Including the recommended generalized gradient diffusion model of
//   Daly and Harlow:
//   \verbatim
//     Daly, B. J., & Harlow, F. H. (1970).
//     Transport equations in turbulence.
//     Physics of Fluids (1958-1988), 13(11), 2634-2649.
//   \endverbatim
//   Optional Gibson-Launder wall-reflection is also provided:
//   \verbatim
//     Gibson, M. M., & Launder, B. E. (1978).
//     Ground effects on pressure fluctuations in the
//     atmospheric boundary layer.
//     Journal of Fluid Mechanics, 86(03), 491-511.
//   \endverbatim
//   The default model coefficients are:
//   \verbatim
//     LRRCoeffs
//     {
//       Cmu             0.09;
//       C1              1.8;
//       C2              0.6;
//       Ceps1           1.44;
//       Ceps2           1.92;
//       Cs              0.25;
//       Ceps            0.15;
//       wallReflection  yes;
//       kappa           0.41
//       Cref1           0.5;
//       Cref2           0.3;
//       couplingFactor  0.0;
//     }
//   \endverbatim
// SourceFiles
//   lrr.cpp
#ifndef lrr_hpp_
#define lrr_hpp_
#include "ras_model.hpp"
#include "reynolds_stress.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class LRR
:
  public ReynoldsStress<RASModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar C1_;
      dimensionedScalar C2_;
      dimensionedScalar Ceps1_;
      dimensionedScalar Ceps2_;
      dimensionedScalar Cs_;
      dimensionedScalar Ceps_;
    // Wall-refection coefficients
      Switch wallReflection_;
      dimensionedScalar kappa_;
      dimensionedScalar Cref1_;
      dimensionedScalar Cref2_;
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
  TYPE_NAME("LRR");
  // Constructors
    //- Construct from components
    LRR
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
    LRR(const LRR&) = delete;
    LRR& operator=(const LRR&) = delete;
  //- Destructor
  virtual ~LRR()
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
#ifdef NoRepository
#   include "lrr.cpp"
#endif
#endif
