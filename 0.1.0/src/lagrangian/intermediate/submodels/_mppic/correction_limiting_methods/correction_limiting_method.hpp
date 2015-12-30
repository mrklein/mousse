// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CorrectionLimitingMethod
// Description
//   Base class for correction limiting methods.
// SourceFiles
//   correction_limiting_method.cpp
#ifndef correction_limiting_method_hpp_
#define correction_limiting_method_hpp_
#include "vol_fields_fwd.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class CorrectionLimitingMethod
{
private:
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const CorrectionLimitingMethod&);
public:
  //- Runtime type information
  TypeName("correctionLimitingMethod");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    CorrectionLimitingMethod,
    dictionary,
    (const dictionary& dict),
    (dict)
  );
  // Constructors
    //- Construct from components
    CorrectionLimitingMethod(const dictionary& dict);
    //- Construct as copy
    CorrectionLimitingMethod(const CorrectionLimitingMethod& cl);
    //- Construct and return a clone
    virtual autoPtr<CorrectionLimitingMethod> clone() const = 0;
  //- Selector
  static autoPtr<CorrectionLimitingMethod> New
  (
    const dictionary& dict
  );
  //- Destructor
  virtual ~CorrectionLimitingMethod();
  // Member Functions
    //- Return the limited velocity
    virtual vector limitedVelocity
    (
      const vector uP,
      const vector dU,
      const vector uMean
    ) const = 0;
};
}  // namespace mousse
#endif
