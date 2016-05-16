#ifndef TURBULENCE_MODELS_PHASE_COMPRESSIBLE_RAS_LAHEY_K_EPSILON_HPP_
#define TURBULENCE_MODELS_PHASE_COMPRESSIBLE_RAS_LAHEY_K_EPSILON_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::LaheyKEpsilon
// Group
//   grpRASTurbulence
// Description
//   Continuous-phase k-epsilon model including bubble-generated turbulence.
//   Reference:
//   \verbatim
//     Lahey Jr, R. T. (2005).
//     The simulation of multidimensional multiphase flows.
//     Nuclear Engineering and Design, 235(10), 1043-1060.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     LaheyKEpsilonCoeffs
//     {
//       Cmu             0.09;
//       C1              1.44;
//       C2              1.92;
//       C3              -0.33;
//       sigmak          1.0;
//       sigmaEps        1.3;
//       Cp              0.25;
//       Cmub            0.6;
//       alphaInversion  0.3;
//     }
//   \endverbatim

#include "k_epsilon.hpp"


namespace mousse {
namespace RASModels {

template<class BasicTurbulenceModel>
class LaheyKEpsilon
:
  public kEpsilon<BasicTurbulenceModel>
{
  // Private data
    mutable const PhaseCompressibleTurbulenceModel
    <
      typename BasicTurbulenceModel::transportModel
    > *gasTurbulencePtr_;
  // Private Member Functions
    //- Return the turbulence model for the gas phase
    const PhaseCompressibleTurbulenceModel
    <
      typename BasicTurbulenceModel::transportModel
    >&
    gasTurbulence() const;
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar alphaInversion_;
      dimensionedScalar Cp_;
      dimensionedScalar C3_;
      dimensionedScalar Cmub_;
  // Protected Member Functions
    virtual void correctNut();
    tmp<volScalarField> bubbleG() const;
    tmp<volScalarField> phaseTransferCoeff() const;
    virtual tmp<fvScalarMatrix> kSource() const;
    virtual tmp<fvScalarMatrix> epsilonSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("LaheyKEpsilon");
  // Constructors
    //- Construct from components
    LaheyKEpsilon
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
    LaheyKEpsilon(const LaheyKEpsilon&) = delete;
    LaheyKEpsilon& operator=(const LaheyKEpsilon&) = delete;
  //- Destructor
  virtual ~LaheyKEpsilon()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};

}  // namespace RASModels
}  // namespace mousse

#include "lahey_k_epsilon.ipp"

#endif
