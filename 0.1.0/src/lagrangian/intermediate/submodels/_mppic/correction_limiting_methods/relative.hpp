// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CorrectionLimitingMethods::relative
// Description
//   Correction limiting method based on the relative particle velocity.
//   This method limits the velocity correction to that of a rebound with a
//   coefficient of restitution \f$e\f$. The relative velocity of the particle
//   with respect to the mean value is used to calculate the direction and
//   magnitude of the limited velocity.
// SourceFiles
//   relative.cpp
#ifndef relative_hpp_
#define relative_hpp_
#include "correction_limiting_method.hpp"
namespace mousse
{
namespace CorrectionLimitingMethods
{
class relative
:
  public CorrectionLimitingMethod
{
protected:
  // Protected data
    //- Coefficient of restitution
    scalar e_;
public:
  //- Runtime type information
  TypeName("relative");
  // Constructors
    //- Construct from components
    relative(const dictionary& dict);
    //- Construct as copy
    relative(const relative& cl);
    //- Construct and return a clone
    virtual autoPtr<CorrectionLimitingMethod> clone() const
    {
      return autoPtr<CorrectionLimitingMethod>
      (
        new relative(*this)
      );
    }
  //- Destructor
  virtual ~relative();
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
