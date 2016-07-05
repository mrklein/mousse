#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TIME_ACTIVATED_FILE_UPDATE_TIME_ACTIVATED_FILE_UPDATE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TIME_ACTIVATED_FILE_UPDATE_TIME_ACTIVATED_FILE_UPDATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::timeActivatedFileUpdate
// Group
//   grpUtilitiesFunctionObjects
// Description
//   Performs a file copy/replacement once a specified time has been reached.
//   Example usage to update the fvSolution dictionary at various times
//   throughout the calculation:
//   \verbatim
//   fileUpdate1
//   {
//     type              timeActivatedFileUpdate;
//     functionObjectLibs ("libutilityFunctionObjects.so");
//     outputControl     timeStep;
//     outputInterval    1;
//     fileToUpdate      "$MOUSSE_CASE/system/fvSolution";
//     timeVsFile
//     (
//       (-1 "$MOUSSE_CASE/system/fvSolution.0")
//       (0.10 "$MOUSSE_CASE/system/fvSolution.10")
//       (0.20 "$MOUSSE_CASE/system/fvSolution.20")
//       (0.35 "$MOUSSE_CASE/system/fvSolution.35")
//     );
//   }
//   \endverbatim

#include "tuple2.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;


class timeActivatedFileUpdate
{
  // Private data
    //- Name of this set of timeActivatedFileUpdate objects
    word name_;
    //- Owner database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of file to update
    fileName fileToUpdate_;
    //- List of times vs filenames
    List<Tuple2<scalar, fileName>> timeVsFile_;
    //- Index of last file copied
    label lastIndex_;
  // Private Member Functions
    //- Update file
    void updateFile();
public:
  //- Runtime type information
  TYPE_NAME("timeActivatedFileUpdate");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    timeActivatedFileUpdate
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    timeActivatedFileUpdate(const timeActivatedFileUpdate&) = delete;
    //- Disallow default bitwise assignment
    timeActivatedFileUpdate& operator=
    (
      const timeActivatedFileUpdate&
    ) = delete;
  //- Destructor
  virtual ~timeActivatedFileUpdate();
  // Member Functions
    //- Return name of the set of timeActivatedFileUpdate
    virtual const word& name() const { return name_; }
    //- Read the timeActivatedFileUpdate data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the timeActivatedFileUpdate and write
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};

}  // namespace mousse

#endif

