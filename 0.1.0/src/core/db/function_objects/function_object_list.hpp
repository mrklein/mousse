#ifndef CORE_DB_FUNCTION_OBJECTS_FUNCTION_OBJECT_LIST_HPP_
#define CORE_DB_FUNCTION_OBJECTS_FUNCTION_OBJECT_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionObjectList
// Description
//   List of function objects with start(), execute() and end() functions
//   that is called for each object.
// See Also
//   mousse::functionObject and mousse::OutputFilterFunctionObject
// SourceFiles
//   function_object_list.cpp


#include "ptr_list.hpp"
#include "function_object.hpp"
#include "sha1_digest.hpp"
#include "hash_table.hpp"

namespace mousse
{

class mapPolyMesh;

class functionObjectList
:
  private PtrList<functionObject>
{
  // Private data

    //- A list of SHA1 digests for the function object dictionaries
    List<SHA1Digest> digests_;

    //- Quick lookup of the index into functions/digests
    HashTable<label> indices_;
    const Time& time_;

    //- The parent dictionary containing a "functions" entry
    //  This entry can either be a list or a dictionary of
    //  functionObject specifications.
    const dictionary& parentDict_;

    //- Switch for the execution of the functionObjects
    bool execution_;

    //- Tracks if read() was called while execution is on
    bool updated_;

  // Private Member Functions

    //- Remove and return the function object pointer by name,
    //  and returns the old index via the parameter.
    //  Returns a NULL pointer (and index -1) if it didn't exist.
    functionObject* remove(const word&, label& oldIndex);

public:

  // Constructors

    //- Construct from Time and the execution setting
    //  The functionObject specifications are read from the controlDict
    functionObjectList
    (
      const Time&,
      const bool execution=true
    );

    //- Construct from Time, a dictionary with "functions" entry
    //  and the execution setting.
    //  \param[in]  parentDict - the parent dictionary containing
    //    a "functions" entry, which can either be a list or a dictionary
    //    of functionObject specifications.
    functionObjectList
    (
      const Time&,
      const dictionary& parentDict,
      const bool execution=true
    );

    //- Disallow default bitwise copy construct
    functionObjectList(const functionObjectList&) = delete;

    //- Disallow default bitwise assignment
    functionObjectList& operator=(const functionObjectList&) = delete;

  //- Destructor
  virtual ~functionObjectList();

  // Member Functions

    //- Return the number of elements in the List.
    using PtrList<functionObject>::size;

    //- Return true if the List is empty (ie, size() is zero).
    using PtrList<functionObject>::empty;

    //- Access to the functionObjects
    using PtrList<functionObject>::operator[];

    //- Clear the list of function objects
    virtual void clear();

    //- Find the ID of a given function object by name
    virtual label findObjectID(const word& name) const;

    //- Switch the function objects on
    virtual void on();

    //- Switch the function objects off
    virtual void off();

    //- Return the execution status (on/off) of the function objects
    virtual bool status() const;

    //- Called at the start of the time-loop
    virtual bool start();

    //- Called at each ++ or += of the time-loop. forceWrite overrides
    //  the usual outputControl behaviour and forces writing always
    //  (used in postprocessing mode)
    virtual bool execute(const bool forceWrite = false);

    //- Called when Time::run() determines that the time-loop exits
    virtual bool end();

    //- Called when time was set at the end of the Time::operator++
    virtual bool timeSet();

    //- Called at the end of Time::adjustDeltaT() if adjustTime is true
    virtual bool adjustTimeStep();

    //- Read and set the function objects if their data have changed
    virtual bool read();

    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh& mpm);

    //- Update for changes of mesh
    virtual void movePoints(const polyMesh& mesh);

};

}  // namespace mousse

#endif
