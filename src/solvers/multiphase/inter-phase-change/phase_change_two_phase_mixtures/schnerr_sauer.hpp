#ifndef SCHNERR_SAUER_HPP_
#define SCHNERR_SAUER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseChangeTwoPhaseMixtures::SchnerrSauer
// Description
//   SchnerrSauer cavitation model.
//   Reference:
//   \verbatim
//     Schnerr, G. H., And Sauer, J.,
//     "Physical and Numerical Modeling of Unsteady Cavitation Dynamics",
//     Proc. 4th International Conference on Multiphase Flow,
//     New Orleans, U.S.A., 2001.
//   \endverbatim

#include "phase_change_two_phase_mixture.hpp"


namespace mousse {
namespace phaseChangeTwoPhaseMixtures {

class SchnerrSauer
:
  public phaseChangeTwoPhaseMixture
{
  // Private data
    //- Bubble number density
    dimensionedScalar n_;
    //- Nucleation site diameter
    dimensionedScalar dNuc_;
    //- Condensation rate coefficient
    dimensionedScalar Cc_;
    //- Vapourisation rate coefficient
    dimensionedScalar Cv_;
    dimensionedScalar p0_;
    //- Nucleation site volume-fraction
    dimensionedScalar alphaNuc() const;
    //- Reciprocal bubble radius
    tmp<volScalarField>rRb(const volScalarField& limitedAlpha1) const;
    //- Part of the condensation and vapourisation rates
    tmp<volScalarField> pCoeff(const volScalarField& p) const;
public:
  //- Runtime type information
  TYPE_NAME("SchnerrSauer");
  // Constructors
    //- Construct from components
    SchnerrSauer
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~SchnerrSauer()
  {}
  // Member Functions
    //- Return the mass condensation and vaporisation rates as a
    //  coefficient to multiply (1 - alphal) for the condensation rate
    //  and a coefficient to multiply  alphal for the vaporisation rate
    virtual Pair<tmp<volScalarField>> mDotAlphal() const;
    //- Return the mass condensation and vaporisation rates as coefficients
    //  to multiply (p - pSat)
    virtual Pair<tmp<volScalarField>> mDotP() const;
    //- Correct the SchnerrSauer phaseChange model
    virtual void correct();
    //- Read the transportProperties dictionary and update
    virtual bool read();
};

}  // namespace phaseChangeTwoPhaseMixtures
}  // namespace mousse

#endif

