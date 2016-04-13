#ifndef CORE_DB_FUNCTION_OBJECTS_OUTPUT_FILTER_FUNCTION_OBJECT_HPP_
#define CORE_DB_FUNCTION_OBJECTS_OUTPUT_FILTER_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OutputFilterFunctionObject
// Description
//   A functionObject wrapper around OutputFilter to allow them to be
//   created via the functions entry within controlDict.
// Note
//   Since the timeIndex is used directly from mousse::Time, it is unaffected
//   by user-time conversions. For example, mousse::engineTime might cause \a
//   writeInterval to be degrees crank angle, but the functionObject
//   execution \a interval would still be in timestep.

#include "function_object.hpp"
#include "dictionary.hpp"
#include "output_filter_output_control.hpp"


namespace mousse {

template<class OutputFilter>
class OutputFilterFunctionObject
:
  public functionObject
{

  // Private data

    //- Reference to the time database
    const Time& time_;

    //- Input dictionary
    dictionary dict_;

    // Optional user inputs

      //- Name of region - defaults to name of polyMesh::defaultRegion
      word regionName_;

      //- Dictionary name to supply required inputs
      word dictName_;

      //- Switch for the execution - defaults to 'yes/on'
      bool enabled_;

      //- Switch to store filter in between writes or use on-the-fly
      //  construction - defaults to true
      bool storeFilter_;

      //- Activation time - defaults to -VGREAT
      scalar timeStart_;

      //- De-activation time - defaults to VGREAT
      scalar timeEnd_;

      //- Number of steps before the dumping time in which the deltaT
      // will start to change (valid for ocAdjustableTime)
      label nStepsToStartTimeChange_;

    //- Output controls
    outputFilterOutputControl outputControl_;

    //- Evaluate controls
    outputFilterOutputControl evaluateControl_;

    //- Pointer to the output filter
    autoPtr<OutputFilter> ptr_;

  // Private Member Functions

    //- Read relevant dictionary entries
    void readDict();

    //- Creates most of the data associated with this object.
    void allocateFilter();

    //- Destroys most of the data associated with this object.
    void destroyFilter();

    //- Returns true if active (enabled and within time bounds)
    bool active() const;

public:

  //- Runtime type information
  TYPE_NAME(OutputFilter::typeName_());

  // Constructors

    //- Construct from components
    OutputFilterFunctionObject
    (
      const word& name,
      const Time&,
      const dictionary&
    );

    //- Disallow default bitwise copy construct
    OutputFilterFunctionObject(const OutputFilterFunctionObject&) = delete;

    //- Disallow default bitwise assignment
    OutputFilterFunctionObject& operator=
    (
      const OutputFilterFunctionObject&
    ) = delete;

  // Member Functions

    // Access

      //- Return time database
      virtual const Time& time() const
      {
        return time_;
      }

      //- Return the input dictionary
      virtual const dictionary& dict() const
      {
        return dict_;
      }

      //- Return the region name
      virtual const word& regionName() const
      {
        return regionName_;
      }

      //- Return the optional dictionary name
      virtual const word& dictName() const
      {
        return dictName_;
      }

      //- Return the enabled flag
      virtual bool enabled() const
      {
        return enabled_;
      }

      //- Return the output control object
      virtual const outputFilterOutputControl& outputControl() const
      {
        return outputControl_;
      }

      //- Return the output filter
      virtual const OutputFilter& outputFilter() const
      {
        return ptr_();
      }

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

#include "output_filter_function_object.ipp"

#endif
