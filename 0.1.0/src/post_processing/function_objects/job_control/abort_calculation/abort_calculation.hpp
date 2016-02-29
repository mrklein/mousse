#ifndef POST_PROCESSING_FUNCTION_OBJECTS_JOB_CONTROL_ABORT_CALCULATION_ABORT_CALCULATION_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_JOB_CONTROL_ABORT_CALCULATION_ABORT_CALCULATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::abortCalculation
// Group
//   grpJobControlFunctionObjects
// Description
//   Watches for presence of the named file in the $FOAM_CASE directory
//   and aborts the calculation if it is present.
//   Currently the following action types are supported:
//   - noWriteNow
//   - writeNow
//   - nextWrite
// SourceFiles
//   abort_calculation.cpp
//   io_abort_calculation.hpp
#include "named_enum.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class abortCalculation
{
public:
  // Public data
    //- Enumeration defining the type of action
    enum actionType
    {
      noWriteNow,    /*!< stop immediately without writing data */
      writeNow,      /*!< write data and stop immediately */
      nextWrite      /*!< stop the next time data are written */
    };
private:
  // Private data
    //- Name of the abort file unless otherwise specified
    word name_;
    const objectRegistry& obr_;
    //- The fully-qualified name of the abort file
    fileName abortFile_;
    //- Action type names
    static const NamedEnum<actionType, 3> actionTypeNames_;
    //- The type of action
    actionType action_;
  // Private Member Functions
    //- Remove abort file.
    void removeFile() const;
public:
  //- Runtime type information
  TYPE_NAME("abort");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    abortCalculation
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFilesUnused = false
    );
    //- Disallow default bitwise copy construct
    abortCalculation(const abortCalculation&) = delete;
    //- Disallow default bitwise assignment
    abortCalculation& operator=(const abortCalculation&) = delete;
  //- Destructor
  virtual ~abortCalculation();
  // Member Functions
    //- Return name of the abort file
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the dictionary settings
    virtual void read(const dictionary&);
    //- Execute, check existence of abort file and take action
    virtual void execute();
    //- Execute at the final time-loop, used for cleanup
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Execute, check existence of abort file and take action
    virtual void write();
    //- Update for changes of mesh - does nothing
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh - does nothing
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#endif
