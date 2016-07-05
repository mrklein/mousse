#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_SET_TIME_STEP_SET_TIME_STEP_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_SET_TIME_STEP_SET_TIME_STEP_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::setTimeStepFunctionObject
// Group
//   grpUtilitiesFunctionObjects
// Description
//   Overrides the timeStep. Can only be used with
//   solvers with adjustTimeStep control (e.g. pimpleFoam). Makes no attempt
//   to cooperate with other timeStep 'controllers' (maxCo, other
//   functionObjects). Supports 'enabled' flag but none of othe other ones
//   'timeStart', 'timeEnd', 'outputControl' etc.

#include "function_object.hpp"
#include "dictionary.hpp"
#include "data_entry.hpp"


namespace mousse {

class setTimeStepFunctionObject
:
  public functionObject
{
  // Private data
    //- Reference to the time database
    const Time& time_;
    // Optional user inputs
      //- Switch for the execution - defaults to 'yes/on'
      bool enabled_;
      //- Time step
      autoPtr<DataEntry<scalar> > timeStepPtr_;
public:
  //- Runtime type information
  TYPE_NAME("setTimeStep");
  // Constructors
    //- Construct from components
    setTimeStepFunctionObject
    (
      const word& name,
      const Time& runTime,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    setTimeStepFunctionObject(const setTimeStepFunctionObject&) = delete;
    //- Disallow default bitwise assignment
    setTimeStepFunctionObject& operator=
    (
      const setTimeStepFunctionObject&
    ) = delete;
  // Member Functions
    // Access
      //- Return time database
      virtual const Time& time() const { return time_; }
      //- Return the enabled flag
      virtual bool enabled() const { return enabled_; }
    // Function object control
      //- Switch the function object on
      virtual void on();
      //- Switch the function object off
      virtual void off();
      //- Called at the start of the time-loop
      virtual bool start();
      //- Called at each ++ or += of the time-loop
      virtual bool execute(const bool forceWrite);
      //- Called when Time::run() determines that the time-loop exits
      virtual bool end();
      //- Called when time was set at the end of the Time::operator++
      virtual bool timeSet();
      //- Called at the end of Time::adjustDeltaT() if adjustTime is true
      virtual bool adjustTimeStep();
      //- Read and set the function object if its data have changed
      virtual bool read(const dictionary&);
      //- Update for changes of mesh
      virtual void updateMesh(const mapPolyMesh& mpm);
      //- Update for changes of mesh
      virtual void movePoints(const polyMesh& mesh);
};

}  // namespace mousse

#endif

