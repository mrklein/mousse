#ifndef COMPRESSIBLEPD_RK_EPSILON_HPP_
#define COMPRESSIBLEPD_RK_EPSILON_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::RASModels::PDRkEpsilon
// Description
//   Standard k-epsilon turbulence model with additional source terms
//   corresponding to PDR basic drag model (\link basic.H \endlink)
//   The default model coefficients correspond to the following:
//   @verbatim
//     PDRkEpsilonCoeffs
//     {
//       Cmu         0.09;
//       C1          1.44;
//       C2          1.92;
//       C3          -0.33;  // only for compressible
//       C4          0.1;
//       sigmak      1.0;    // only for compressible
//       sigmaEps    1.3;
//       Prt         1.0;    // only for compressible
//     }
//   @endverbatim
//   The turbulence source term \f$ G_{R} \f$ appears in the
//   \f$ \kappa-\epsilon \f$ equation for the generation of turbulence due to
//   interaction with unresolved obstacles.
//   In the \f$ \epsilon  \f$ equation \f$ C_{1} G_{R} \f$ is added as a source
//   term.
//   In the \f$ \kappa \f$ equation \f$ G_{R} \f$ is added as a source term.

#include "k_epsilon.hpp"
#include "turbulent_fluid_thermo_model.hpp"


namespace mousse {
namespace compressible {
namespace RASModels {

class PDRkEpsilon
:
  public mousse::RASModels::kEpsilon
  <
    EddyDiffusivity
    <
      compressible::turbulenceModel
    >
  >
{
  // Private data
    // Model coefficients
    dimensionedScalar C4_;
public:
  //- Runtime type information
  TYPE_NAME("PDRkEpsilon");
  // Constructors
    //- Construct from components
    PDRkEpsilon
    (
      const geometricOneField& alpha,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const fluidThermo& thermophysicalModel,
      const word& turbulenceModelName = turbulenceModel::typeName,
      const word& modelName = typeName
    );
  //- Destructor
  virtual ~PDRkEpsilon();
  // Member Functions
    //- Solve the turbulence equations and correct the turbulence viscosity
    void correct();
    //- Read turbulenceProperties dictionary
    bool read();
};

}  // namespace RASModels
}  // namespace compressible
}  // namespace mousse

#endif

