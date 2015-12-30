// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::RNGkEpsilon
// Group
//   grpRASTurbulence
// Description
//   Renormalization group k-epsilon turbulence model for incompressible and
//   compressible flows.
//   Reference:
//   \verbatim
//     Yakhot, V., Orszag, S. A., Thangam, S.,
//     Gatski, T. B., & Speziale, C. G. (1992).
//     Development of turbulence models for shear flows
//     by a double expansion technique.
//     Physics of Fluids A: Fluid Dynamics (1989-1993), 4(7), 1510-1520.
//   For the RDT-based compression term:
//     El Tahry, S. H. (1983).
//     k-epsilon equation for compressible reciprocating engine flows.
//     Journal of Energy, 7(4), 345-353.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     RNGkEpsilonCoeffs
//     {
//       Cmu         0.0845;
//       C1          1.42;
//       C2          1.68;
//       C3          -0.33;
//       sigmak      0.71942;
//       sigmaEps    0.71942;
//       eta0        4.38;
//       beta        0.012;
//     }
//   \endverbatim
// SourceFiles
//   rng_k_epsilon.cpp
#ifndef rng_k_epsilon_hpp_
#define rng_k_epsilon_hpp_
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class RNGkEpsilon
:
  public eddyViscosity<RASModel<BasicTurbulenceModel> >
{
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    RNGkEpsilon(const RNGkEpsilon&);
    RNGkEpsilon& operator=(const RNGkEpsilon&);
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar C1_;
      dimensionedScalar C2_;
      dimensionedScalar C3_;
      dimensionedScalar sigmak_;
      dimensionedScalar sigmaEps_;
      dimensionedScalar eta0_;
      dimensionedScalar beta_;
    // Fields
      volScalarField k_;
      volScalarField epsilon_;
  // Protected Member Functions
    virtual void correctNut();
    virtual tmp<fvScalarMatrix> kSource() const;
    virtual tmp<fvScalarMatrix> epsilonSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TypeName("RNGkEpsilon");
  // Constructors
    //- Construct from components
    RNGkEpsilon
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
  virtual ~RNGkEpsilon()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read();
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DkEff",
          (this->nut_/sigmak_ + this->nu())
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
          (this->nut_/sigmaEps_ + this->nu())
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
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace mousse
#ifdef NoRepository
#   include "rng_k_epsilon.cpp"
#endif
#endif
