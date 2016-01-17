// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::kEpsilon
// Group
//   grpRASTurbulence
// Description
//   Standard k-epsilon turbulence model for incompressible and compressible
//   flows including rapid distortion theory (RDT) based compression term.
//   Reference:
//   \verbatim
//     Standard model:
//       Launder, B. E., & Spalding, D. B. (1972).
//       Lectures in mathematical models of turbulence.
//       Launder, B. E., & Spalding, D. B. (1974).
//       The numerical computation of turbulent flows.
//       Computer methods in applied mechanics and engineering,
//       3(2), 269-289.
//     For the RDT-based compression term:
//       El Tahry, S. H. (1983).
//       k-epsilon equation for compressible reciprocating engine flows.
//       Journal of Energy, 7(4), 345-353.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     kEpsilonCoeffs
//     {
//       Cmu         0.09;
//       C1          1.44;
//       C2          1.92;
//       C3          -0.33;
//       sigmak      1.0;
//       sigmaEps    1.3;
//     }
//   \endverbatim
// SourceFiles
//   k_epsilon.cpp
#ifndef k_epsilon_hpp_
#define k_epsilon_hpp_
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class kEpsilon
:
  public eddyViscosity<RASModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar C1_;
      dimensionedScalar C2_;
      dimensionedScalar C3_;
      dimensionedScalar sigmak_;
      dimensionedScalar sigmaEps_;
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
  TYPE_NAME("kEpsilon");
  // Constructors
    //- Construct from components
    kEpsilon
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
    kEpsilon(const kEpsilon&) = delete;
    kEpsilon& operator=(const kEpsilon&) = delete;
  //- Destructor
  virtual ~kEpsilon()
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
#   include "k_epsilon.cpp"
#endif
#endif
