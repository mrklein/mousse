// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::continuousGasKEpsilon
// Group
//   grpRASTurbulence
// Description
//   k-epsilon model for the gas-phase in a two-phase system
//   supporting phase-inversion.
//   In the limit that the gas-phase fraction approaches zero a contribution from
//   the other phase is blended into the k and epsilon equations up to the
//   phase-fraction of alphaInversion at which point phase-inversion is
//   considered to have occurred and the model reverts to the pure single-phase
//   form.
//   This model is unpublished and is provided as a stable numerical framework
//   on which a more physical model may be built.
//   The default model coefficients are
//   \verbatim
//     continuousGasKEpsilonCoeffs
//     {
//       Cmu             0.09;
//       C1              1.44;
//       C2              1.92;
//       C3              -0.33;
//       sigmak          1.0;
//       sigmaEps        1.3;
//       alphaInversion  0.7;
//     }
//   \endverbatim
// SourceFiles
//   continuous_gas_k_epsilon.cpp
#ifndef continuous_gas_k_epsilon_hpp_
#define continuous_gas_k_epsilon_hpp_
#include "k_epsilon.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class continuousGasKEpsilon
:
  public kEpsilon<BasicTurbulenceModel>
{
  // Private data
    mutable const turbulenceModel *liquidTurbulencePtr_;
    volScalarField nutEff_;
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    continuousGasKEpsilon(const continuousGasKEpsilon&);
    continuousGasKEpsilon& operator=(const continuousGasKEpsilon&);
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar alphaInversion_;
  // Protected Member Functions
    virtual void correctNut();
    tmp<volScalarField> phaseTransferCoeff() const;
    virtual tmp<fvScalarMatrix> kSource() const;
    virtual tmp<fvScalarMatrix> epsilonSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TypeName("continuousGasKEpsilon");
  // Constructors
    //- Construct from components
    continuousGasKEpsilon
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
  virtual ~continuousGasKEpsilon()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read();
    //- Return the turbulence model for the liquid phase
    const turbulenceModel& liquidTurbulence() const;
    //- Return the effective viscosity
    virtual tmp<volScalarField> nuEff() const;
    //- Return the effective density for the stress
    virtual tmp<volScalarField> rhoEff() const;
    //- Return the Reynolds stress tensor
    virtual tmp<volSymmTensorField> R() const;
};
}  // namespace RASModels
}  // namespace mousse
#ifdef NoRepository
#   include "continuous_gas_k_epsilon.cpp"
#endif
#endif
