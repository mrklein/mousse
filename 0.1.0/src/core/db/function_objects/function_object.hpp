#ifndef CORE_DB_FUNCTION_OBJECTS_FUNCTION_OBJECT_HPP_
#define CORE_DB_FUNCTION_OBJECTS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionObject
// Description
//   Abstract base-class for Time/database function objects.
// See Also
//   mousse::OutputFilterFunctionObject

#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

// Forward declaration of classes
class Time;
class polyMesh;
class mapPolyMesh;


class functionObject
{
  // Private data

    //- Name
    const word name_;

public:

  //- Runtime type information
  virtual const word& type() const = 0;

  static int debug;

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      functionObject,
      dictionary,
      (const word& name, const Time& t, const dictionary& dict),
      (name, t, dict)
    );

  // Constructors

    //- Construct from components
    functionObject(const word& name);

    //- Return clone
    autoPtr<functionObject> clone() const
    {
      NOT_IMPLEMENTED("functionObject::clone() const");
      return autoPtr<functionObject>(NULL);
    }

    //- Return a pointer to a new functionObject created on freestore
    //  from Istream
    class iNew
    {
      const Time& time_;
    public:
      iNew(const Time& t)
      :
        time_(t)
      {}

      autoPtr<functionObject> operator()
      (
        const word& name,
        Istream& is
      ) const;
    };

    //- Disallow default bitwise copy construct
    functionObject(const functionObject&) = delete;

    //- Disallow default bitwise assignment
    void operator=(const functionObject&) = delete;

  // Selectors

    //- Select from dictionary, based on its "type" entry
    static autoPtr<functionObject> New
    (
      const word& name,
      const Time&,
      const dictionary&
    );

  //- Destructor
  virtual ~functionObject();

  // Member Functions

    //- Name
    virtual const word& name() const;

    //- Called at the start of the time-loop
    virtual bool start() = 0;

    //- Called at each ++ or += of the time-loop. forceWrite overrides the
    //  outputControl behaviour.
    virtual bool execute(const bool forceWrite) = 0;

    //- Called when Time::run() determines that the time-loop exits.
    //  By default it simply calls execute().
    virtual bool end();

    //- Called when time was set at the end of the Time::operator++
    virtual bool timeSet();

    //- Called at the end of Time::adjustDeltaT() if adjustTime is true
    virtual bool adjustTimeStep();

    //- Read and set the function object if its data have changed
    virtual bool read(const dictionary&) = 0;

    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh& mpm) = 0;

    //- Update for changes of mesh
    virtual void movePoints(const polyMesh& mesh) = 0;

};

}  // namespace mousse

#endif
