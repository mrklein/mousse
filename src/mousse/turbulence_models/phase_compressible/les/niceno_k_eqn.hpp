#ifndef TURBULENCE_MODELS_PHASE_COMPRESSIBLE_LES_NICENO_K_EQN_HPP_
#define TURBULENCE_MODELS_PHASE_COMPRESSIBLE_LES_NICENO_K_EQN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::NicenoKEqn
// Group
//   grpLESTurbulence
// Description
//   One-equation SGS model for the continuous phase in a two-phase system
//   including bubble-generated turbulence.
//   Reference:
//   \verbatim
//     Niceno, B., Dhotre, M. T., & Deen, N. G. (2008).
//     One-equation sub-grid scale (SGS) modelling for
//     Euler–Euler large eddy simulation (EELES) of dispersed bubbly flow.
//     Chemical Engineering Science, 63(15), 3923-3931.
//   \endverbatim
//   The default model coefficients are:
//   \verbatim
//     NicenoKEqnCoeffs
//     {
//       Ck              0.094;
//       Ce              1.048;
//       alphaInversion  0.3;
//       Cp              Ck;
//       Cmub            0.6;
//     }
//   \endverbatim

#include "k_eqn.hpp"


namespace mousse {
namespace LESModels {

template<class BasicTurbulenceModel>
class NicenoKEqn
:
  public kEqn<BasicTurbulenceModel>
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
      dimensionedScalar Cmub_;
  // Protected Member Functions
    virtual void correctNut();
    tmp<volScalarField> bubbleG() const;
    tmp<volScalarField> phaseTransferCoeff() const;
    virtual tmp<fvScalarMatrix> kSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("NicenoKEqn");
  // Constructors
    //- Construct from components
    NicenoKEqn
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
    NicenoKEqn(const NicenoKEqn&) = delete;
    NicenoKEqn& operator=(const NicenoKEqn&) = delete;
  //- Destructor
  virtual ~NicenoKEqn()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
};

}  // namespace LESModels
}  // namespace mousse

#include "niceno_k_eqn.ipp"

#endif
