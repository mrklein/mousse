// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::continuousGasKEqn
// Group
//   grpLESTurbulence
// Description
//   One-equation SGS model for the gas-phase in a two-phase system
//   supporting phase-inversion.
//   In the limit that the gas-phase fraction approaches zero a contribution from
//   the other phase is blended into the k-equation up to the phase-fraction of
//   alphaInversion at which point phase-inversion is considered to have occurred
//   and the model reverts to the pure single-phase form.
//   This model is unpublished and is provided as a stable numerical framework
//   on which a more physical model may be built.
//   The default model coefficients are
//   \verbatim
//     continuousKEqnCoeffs
//     {
//       Ck              0.094;
//       Ce              1.048;
//       alphaInversion  0.7;
//     }
//   \endverbatim
// SourceFiles
//   continuous_gas_k_eqn.cpp
#ifndef continuous_gas_k_eqn_hpp_
#define continuous_gas_k_eqn_hpp_
#include "k_eqn.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class continuousGasKEqn
:
  public kEqn<BasicTurbulenceModel>
{
  // Private data
    mutable const turbulenceModel *liquidTurbulencePtr_;
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar alphaInversion_;
  // Protected Member Functions
    //- Return the turbulence model for the liquid phase
    const turbulenceModel& liquidTurbulence() const;
    tmp<volScalarField> phaseTransferCoeff() const;
    virtual tmp<fvScalarMatrix> kSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("continuousGasKEqn");
  // Constructors
    //- Construct from components
    continuousGasKEqn
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
    continuousGasKEqn(const continuousGasKEqn&) = delete;
    continuousGasKEqn& operator=(const continuousGasKEqn&) = delete;
  //- Destructor
  virtual ~continuousGasKEqn()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "continuous_gas_k_eqn.cpp"
#endif
#endif
