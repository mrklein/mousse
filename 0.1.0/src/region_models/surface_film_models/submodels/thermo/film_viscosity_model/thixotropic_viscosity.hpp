// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thixotropicViscosity
// Description
//   Thixotropic viscosity model based on the evolution of the structural
//   parameter \f$ \lambda \f$:
//     \f[
//       \lambda = a(1 - \lambda)^b - c \lambda \dot{\gamma}^d
//     \f]
//   The viscosity is then calculated using the expression
//     \f[
//       \mu = \frac{\mu_{\infty}}{{1 - K \lambda}^2}
//     \f]
//   Where the parameter K is given by:
//     \f[
//       K = 1 - \sqrt{\frac{\mu_{\infty}}{\mu_{0}}}
//     \f]
//   Here:
//   \vartable
//     \lambda         | structural parameter
//     a               | model coefficient
//     b               | model coefficient
//     c               | model coefficient
//     d               | model coefficient
//     \dot{\gamma}    | stress rate [1/s]
//     \mu_{0}         | limiting viscosity when \f$ \lambda = 1 \f$
//     \mu_{\infty}    | limiting viscosity when \f$ \lambda = 0 \f$
//   \endvartable
//   Reference:
//   \verbatim
//     Barnes H A, 1997.  Thixotropy - a review.  J. Non-Newtonian Fluid
//     Mech 70, pp 1-33
//   \endverbatim
// SourceFiles
//   thixotropic_viscosity.cpp
#ifndef thixotropic_viscosity_hpp_
#define thixotropic_viscosity_hpp_
#include "film_viscosity_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class thixotropicViscosity
:
  public filmViscosityModel
{
protected:
  // Protected data
    //- Model `a' coefficient
    dimensionedScalar a_;
    //- Model `b' coefficient
    dimensionedScalar b_;
    //- Model `d' coefficient
    dimensionedScalar d_;
    //- Model `c' coefficient (read after d since dims depend on d value)
    dimensionedScalar c_;
    //- Limiting viscosity when lambda = 1
    dimensionedScalar mu0_;
    //- Limiting viscosity when lambda = 0
    dimensionedScalar muInf_;
    //- Model coeffiicient
    dimensionedScalar K_;
    //- Structural parameter
    //  0 = freestream value (most liquid)
    //  1 = fully built (most solid)
    volScalarField lambda_;
public:
  //- Runtime type information
  TYPE_NAME("thixotropic");
  // Constructors
    //- Construct from surface film model
    thixotropicViscosity
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
    //- Disallow default bitwise copy construct
    thixotropicViscosity(const thixotropicViscosity&) = delete;
    //- Disallow default bitwise assignment
    thixotropicViscosity& operator=(const thixotropicViscosity&) = delete;
  //- Destructor
  virtual ~thixotropicViscosity();
  // Member Functions
    //- Correct
    virtual void correct
    (
      const volScalarField& p,
      const volScalarField& T
    );
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
