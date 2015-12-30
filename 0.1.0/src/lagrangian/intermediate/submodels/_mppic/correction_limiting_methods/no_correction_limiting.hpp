// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CorrectionLimitingMethods::noCorrectionLimiting
// Description
// SourceFiles
//   no_correction_limiting.cpp
#ifndef no_correction_limiting_hpp_
#define no_correction_limiting_hpp_
#include "correction_limiting_method.hpp"
namespace mousse
{
namespace CorrectionLimitingMethods
{
class noCorrectionLimiting
:
  public CorrectionLimitingMethod
{
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from components
    noCorrectionLimiting(const dictionary& dict);
    //- Construct as copy
    noCorrectionLimiting(const noCorrectionLimiting& cl);
    //- Construct and return a clone
    virtual autoPtr<CorrectionLimitingMethod> clone() const
    {
      return autoPtr<CorrectionLimitingMethod>
      (
        new noCorrectionLimiting(*this)
      );
    }
  //- Destructor
  virtual ~noCorrectionLimiting();
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
