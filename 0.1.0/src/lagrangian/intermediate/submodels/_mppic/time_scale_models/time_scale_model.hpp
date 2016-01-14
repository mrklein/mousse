// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TimeScaleModel
// Description
//   Base class for time scale models.
// SourceFiles
//   time_scale_model.cpp
#ifndef time_scale_model_hpp_
#define time_scale_model_hpp_
#include "constants.hpp"
#include "field.hpp"
#include "field_field.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class TimeScaleModel
{
protected:
  // Protected data
    //- Close pack volume fraction
    scalar alphaPacked_;
    //- Coefficient of restitution
    scalar e_;
public:
  //- Runtime type information
  TYPE_NAME("timeScaleModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    TimeScaleModel,
    dictionary,
    (const dictionary& dict),
    (dict)
  );
  //- Constructors
    //- Construct from components
    TimeScaleModel(const dictionary& dict);
    //- Construct a copy
    TimeScaleModel(const TimeScaleModel& sm);
    //- Construct and return a clone
    virtual autoPtr<TimeScaleModel> clone() const = 0;
    //- Disallow default bitwise assignment
    TimeScaleModel& operator=(const TimeScaleModel&) = delete;
  //- Selector
  static autoPtr<TimeScaleModel> New
  (
    const dictionary& dict
  );
  //- Destructor
  virtual ~TimeScaleModel();
  //- Member Functions
    //- Time scale
    virtual tmp<FieldField<Field, scalar> > oneByTau
    (
      const FieldField<Field, scalar>& alpha,
      const FieldField<Field, scalar>& r32,
      const FieldField<Field, scalar>& uSqr,
      const FieldField<Field, scalar>& f
    ) const = 0;
};
}  // namespace mousse
#endif
