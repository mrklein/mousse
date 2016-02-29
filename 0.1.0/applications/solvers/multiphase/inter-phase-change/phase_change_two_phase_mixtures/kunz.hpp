// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseChangeTwoPhaseMixtures::Kunz
// Description
//   Kunz cavitation model slightly modified so that the condensation term
//   is switched off when the pressure is less than the saturation vapour
//   pressure.  This change allows the condensation term to be formulated as
//   a coefficient multiplying (p - p_sat) so that it can be included as an
//   implicit term in the pressure equation.
//   Reference:
//   \verbatim
//     Kunz, R.F., Boger, D.A., Stinebring, D.R., Chyczewski, Lindau. J.W.,
//     Gibeling, H.J., Venkateswaran, S., Govindan, T.R.,
//     "A Preconditioned Implicit Method for Two-Phase Flows with Application
//     to Cavitation Prediction,"
//     Computers and Fluids,
//     29(8):849-875, 2000.
//   \endverbatim
// SourceFiles
//   kunz.cpp
#ifndef KUNZ_HPP_
#define KUNZ_HPP_
#include "phase_change_two_phase_mixture.hpp"
// * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * //
namespace mousse
{
namespace phaseChangeTwoPhaseMixtures
{
class Kunz
:
  public phaseChangeTwoPhaseMixture
{
  // Private data
    dimensionedScalar UInf_;
    dimensionedScalar tInf_;
    dimensionedScalar Cc_;
    dimensionedScalar Cv_;
    dimensionedScalar p0_;
    dimensionedScalar mcCoeff_;
    dimensionedScalar mvCoeff_;
public:
  //- Runtime type information
  TYPE_NAME("Kunz");
  // Constructors
    //- Construct from components
    Kunz
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~Kunz()
  {}
  // Member Functions
    //- Return the mass condensation and vaporisation rates as a
    //  coefficient to multiply (1 - alphal) for the condensation rate
    //  and a coefficient to multiply  alphal for the vaporisation rate
    virtual Pair<tmp<volScalarField>> mDotAlphal() const;
    //- Return the mass condensation and vaporisation rates as coefficients
    //  to multiply (p - pSat)
    virtual Pair<tmp<volScalarField>> mDotP() const;
    //- Correct the Kunz phaseChange model
    virtual void correct();
    //- Read the transportProperties dictionary and update
    virtual bool read();
};
}  // namespace phaseChangeTwoPhaseMixtures
}  // namespace mousse
#endif
