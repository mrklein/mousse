// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::mixtureKEpsilon
// Group
//   grpRASTurbulence
// Description
//   Mixture k-epsilon turbulence model for two-phase gas-liquid systems
//   The basic structure of the model is based on:
//   \verbatim
//     Behzadi, A., Issa, R. I., & Rusche, H. (2004).
//     Modelling of dispersed bubble and droplet flow at high phase fractions.
//     Chemical Engineering Science, 59(4), 759-770.
//   \endverbatim
//   but an effective density for the gas is used in the averaging and an
//   alternative model for bubble-generated turbulence from:
//   \verbatim
//     Lahey Jr, R. T. (2005).
//     The simulation of multidimensional multiphase flows.
//     Nuclear Engineering and Design, 235(10), 1043-1060.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     mixtureKEpsilonCoeffs
//     {
//       Cmu         0.09;
//       C1          1.44;
//       C2          1.92;
//       C3          C2;
//       sigmak      1.0;
//       sigmaEps    1.3;
//     }
//   \endverbatim
// SourceFiles
//   mixture_k_epsilon.cpp
#ifndef mixture_k_epsilon_hpp_
#define mixture_k_epsilon_hpp_
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class mixtureKEpsilon
:
  public eddyViscosity<RASModel<BasicTurbulenceModel> >
{
  // Private data
    mutable mixtureKEpsilon<BasicTurbulenceModel> *liquidTurbulencePtr_;
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    mixtureKEpsilon(const mixtureKEpsilon&);
    mixtureKEpsilon& operator=(const mixtureKEpsilon&);
    //- Return the turbulence model for the other phase
    mixtureKEpsilon<BasicTurbulenceModel>& liquidTurbulence() const;
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar C1_;
      dimensionedScalar C2_;
      dimensionedScalar C3_;
      dimensionedScalar Cp_;
      dimensionedScalar sigmak_;
      dimensionedScalar sigmaEps_;
    // Fields
      volScalarField k_;
      volScalarField epsilon_;
    // Mixture fields
      autoPtr<volScalarField> Ct2_;
      autoPtr<volScalarField> rhom_;
      autoPtr<volScalarField> km_;
      autoPtr<volScalarField> epsilonm_;
  // Protected Member Functions
    wordList epsilonBoundaryTypes(const volScalarField& epsilon) const;
    void correctInletOutlet
    (
      volScalarField& vsf,
      const volScalarField& refVsf
    ) const;
    void initMixtureFields();
    virtual void correctNut();
    tmp<volScalarField> Ct2() const;
    tmp<volScalarField> rholEff() const;
    tmp<volScalarField> rhogEff() const;
    tmp<volScalarField> rhom() const;
    tmp<volScalarField> mix
    (
      const volScalarField& fc,
      const volScalarField& fd
    ) const;
    tmp<volScalarField> mixU
    (
      const volScalarField& fc,
      const volScalarField& fd
    ) const;
    tmp<surfaceScalarField> mixFlux
    (
      const surfaceScalarField& fc,
      const surfaceScalarField& fd
    ) const;
    tmp<volScalarField> bubbleG() const;
    virtual tmp<fvScalarMatrix> kSource() const;
    virtual tmp<fvScalarMatrix> epsilonSource() const;
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff(const volScalarField& nutm) const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DkEff",
          nutm/sigmak_
        )
      );
    }
    //- Return the effective diffusivity for epsilon
    tmp<volScalarField> DepsilonEff(const volScalarField& nutm) const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DepsilonEff",
          nutm/sigmaEps_
        )
      );
    }
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TypeName("mixtureKEpsilon");
  // Constructors
    //- Construct from components
    mixtureKEpsilon
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
  virtual ~mixtureKEpsilon()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
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
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace mousse
#ifdef NoRepository
#   include "mixture_k_epsilon.cpp"
#endif
#endif
