#ifndef THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_RAVI_PETERSEN_HPP_
#define THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_RAVI_PETERSEN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminarFlameSpeedModels::RaviPetersen
// Description
//   Laminar flame speed obtained from Ravi and Petersen's correlation.
//   The correlation for the laminar flame speed \f$Su\f$ is of the following
//   form:
//   \f[
//     Su = \left( \sum \alpha_i \phi^i \right)
//     \left( \frac{T}{T_{ref}} \right)^{\left( \sum \beta_j \phi^j \right)}
//   \f]
//   Where \f$\phi\f$ is the equivalence ratio, and \f$\alpha\f$ and \f$\beta\f$
//   are polynomial coefficients given for a number of pressure and equivalence
//   ratio points.
// SourceFiles
//   ravi_petersen.cpp
#include "laminar_flame_speed.hpp"
namespace mousse
{
namespace laminarFlameSpeedModels
{
class RaviPetersen
:
  public laminarFlameSpeed
{
  // Private Data
    dictionary coeffsDict_;
    //- Correlation pressure values
    List<scalar> pPoints_;
    //- Correlation equivalence ratios
    List<scalar> EqRPoints_;
    //- Correlation alpha coefficients
    List<List<List<scalar> > > alpha_;
    //- Correlation beta coefficients
    List<List<List<scalar> > > beta_;
    //- Reference temperature
    scalar TRef_;
  // Private Member Functions
    //- Check that input points are ordered
    void checkPointsMonotonicity
    (
      const word& name,
      const List<scalar>& x
    ) const;
    //- Check that the coefficient arrays are of the correct shape
    void checkCoefficientArrayShape
    (
      const word& name,
      const List<List<List<scalar> > >& x
    ) const;
    //- Find and interpolate a value in the data point arrays
    inline bool interval
    (
      const List<scalar>& xPoints,
      const scalar x,
      label& xIndex,
      scalar& xXi,
      scalar& xLim
    ) const;
    //- Evaluate a polynomial
    inline scalar polynomial
    (
      const List<scalar>& coeffs,
      const scalar x
    ) const;
    //- Evaluate a polynomial differential
    inline scalar dPolynomial
    (
      const List<scalar>& coeffs,
      const scalar x
    ) const;
    //- Calculate normalised temperature to the power of the B polynomial
    inline scalar THatPowB
    (
      const label EqRIndex,
      const label pIndex,
      const scalar EqR,
      const scalar Tu
    ) const;
    //- Return the flame speed within the correlation range
    inline scalar correlationInRange
    (
      const label EqRIndex,
      const label pIndex,
      const scalar EqR,
      const scalar Tu
    ) const;
    //- Extrapolate the flame speed correlation outside its range
    inline scalar correlationOutOfRange
    (
      const label EqRIndex,
      const label pIndex,
      const scalar EqR,
      const scalar EqRLim,
      const scalar Tu
    ) const;
    //- Return the laminar flame speed [m/s]
    inline scalar speed
    (
      const scalar EqR,
      const scalar p,
      const scalar Tu
    ) const;
    //- Construct as copy (not implemented)
    RaviPetersen(const RaviPetersen&);
    void operator=(const RaviPetersen&);
public:
  //- Runtime type information
  TYPE_NAME("RaviPetersen");
  // Constructors
    //- Construct from dictionary and psiuReactionThermo
    RaviPetersen
    (
      const dictionary&,
      const psiuReactionThermo&
    );
  //- Destructor
  virtual ~RaviPetersen();
  // Member functions
    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;
};
} // End laminarFlameSpeedModels
}  // namespace mousse
#endif
