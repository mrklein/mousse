#ifndef MERKLE_HPP_
#define MERKLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseChangeTwoPhaseMixtures::Merkle
// Description
//   Merkle cavitation model.
//   Reference:
//   \verbatim
//     C. L. Merkle, J. Feng, and P. E. O. Buelow,
//     "Computational modeling of the dynamics of sheet cavitation",
//     in Proceedings Third International Symposium on Cavitation
//     Grenoble, France 1998.
//  \endverbatim

#include "phase_change_two_phase_mixture.hpp"


namespace mousse {
namespace phaseChangeTwoPhaseMixtures {

class Merkle
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
  TYPE_NAME("Merkle");
  // Constructors
    //- Construct from components
    Merkle
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~Merkle()
  {}
  // Member Functions
    //- Return the mass condensation and vaporisation rates as a
    //  coefficient to multiply (1 - alphal) for the condensation rate
    //  and a coefficient to multiply  alphal for the vaporisation rate
    virtual Pair<tmp<volScalarField>> mDotAlphal() const;
    //- Return the mass condensation and vaporisation rates as coefficients
    //  to multiply (p - pSat)
    virtual Pair<tmp<volScalarField>> mDotP() const;
    //- Correct the Merkle phaseChange model
    virtual void correct();
    //- Read the transportProperties dictionary and update
    virtual bool read();
};

}  // namespace phaseChangeTwoPhaseMixtures
}  // namespace mousse

#endif

