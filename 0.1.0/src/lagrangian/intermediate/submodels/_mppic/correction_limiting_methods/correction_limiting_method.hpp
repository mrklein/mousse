#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_CORRECTION_LIMITING_METHODS_CORRECTION_LIMITING_METHOD_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_CORRECTION_LIMITING_METHODS_CORRECTION_LIMITING_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CorrectionLimitingMethod
// Description
//   Base class for correction limiting methods.

#include "vol_fields_fwd.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class CorrectionLimitingMethod
{
public:
  //- Runtime type information
  TYPE_NAME("correctionLimitingMethod");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
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
    //- Disallow default bitwise assignment
    CorrectionLimitingMethod& operator=
    (
      const CorrectionLimitingMethod&
    ) = delete;
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

