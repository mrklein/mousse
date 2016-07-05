#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_CORRECTION_LIMITING_METHODS_ABSOLUTE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_CORRECTION_LIMITING_METHODS_ABSOLUTE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CorrectionLimitingMethods::absolute
// Description
//   Correction limiting method based on the absolute particle velocity.
//   This method that limits the velocity correction to that of a rebound with a
//   coefficient of restitution \f$e\f$. The absolute velocity of the particle
//   is used when calculating the magnitude of the limited correction.
//   The direction is calculated using the relative velocity.

#include "correction_limiting_method.hpp"


namespace mousse {
namespace CorrectionLimitingMethods {

class absolute
:
  public CorrectionLimitingMethod
{
protected:
  // Protected data
    //- Coefficient of restitution
    scalar e_;
public:
  //- Runtime type information
  TYPE_NAME("absolute");
  // Constructors
    //- Construct from components
    absolute(const dictionary& dict);
    //- Construct as copy
    absolute(const absolute& cl);
    //- Construct and return a clone
    virtual autoPtr<CorrectionLimitingMethod> clone() const
    {
      return
        autoPtr<CorrectionLimitingMethod>
        {
          new absolute{*this}
        };
    }
  //- Destructor
  virtual ~absolute();
  // Member Functions
    //- Return the limited velocity
    virtual vector limitedVelocity
    (
      const vector uP,
      const vector dU,
      const vector uMean
    ) const;
};

}  // namespace CorrectionLimitingMethods
}  // namespace mousse

#endif

