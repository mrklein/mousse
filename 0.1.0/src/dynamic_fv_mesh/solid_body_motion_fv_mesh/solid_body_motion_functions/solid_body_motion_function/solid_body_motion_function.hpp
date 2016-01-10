// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunction
// Description
//   Base class for defining solid-body motions
// SourceFiles
//   solid_body_motion_function.cpp
//   dynamic_fv_mesh_new.cpp
#ifndef solid_body_motion_function_hpp_
#define solid_body_motion_function_hpp_
#include "time.hpp"
#include "dictionary.hpp"
#include "septernion.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class solidBodyMotionFunction
{
protected:
  // Protected data
    dictionary SBMFCoeffs_;
    const Time& time_;
public:
  //- Runtime type information
  TYPE_NAME("solidBodyMotionFunction");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      solidBodyMotionFunction,
      dictionary,
      (const dictionary& SBMFCoeffs, const Time& runTime),
      (SBMFCoeffs, runTime)
    );
  // Constructors
    //- Construct from the SBMFCoeffs dictionary and Time
    solidBodyMotionFunction
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const = 0;
    //- Disallow default bitwise copy construct
    solidBodyMotionFunction(const solidBodyMotionFunction&) = delete;
    //- Disallow default bitwise assignment
    solidBodyMotionFunction& operator=(const solidBodyMotionFunction&) = delete;
  // Selectors
    //- Select constructed from the SBMFCoeffs dictionary and Time
    static autoPtr<solidBodyMotionFunction> New
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
  //- Destructor
  virtual ~solidBodyMotionFunction();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const = 0;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs) = 0;
    //- Write in dictionary format
    virtual void writeData(Ostream&) const;
};
}  // namespace mousse
#endif
