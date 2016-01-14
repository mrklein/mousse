// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldValue
// Group
//   grpFieldFunctionObjects
// Description
//   Base class for field value -based function objects.
// SourceFiles
//   field_value.cpp
#ifndef field_value_hpp_
#define field_value_hpp_
#include "function_object_file.hpp"
#include "switch.hpp"
#include "ofstream.hpp"
#include "dictionary.hpp"
#include "field.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class fvMesh;
class polyMesh;
class mapPolyMesh;
class fieldValue
:
  public functionObjectFile
{
protected:
  // Protected data
    //- Name of this fieldValue object
    word name_;
    //- Database this class is registered to
    const objectRegistry& obr_;
    //- Construction dictionary
    dictionary dict_;
    //- Active flag
    bool active_;
    //- Switch to send output to Info as well as to file
    Switch log_;
    //- Name of source object
    word sourceName_;
    //- List of field names to operate on
    wordList fields_;
    //- Output field values flag
    Switch valueOutput_;
    //- Results dictionary for external access of results
    dictionary resultDict_;
public:
  //- Run-time type information
  TYPE_NAME("fieldValue");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      fieldValue,
      dictionary,
      (
        const word& name,
        const objectRegistry& obr,
        const dictionary& dict,
        const bool loadFromFiles
      ),
      (name, obr, dict, loadFromFiles)
    );
  //- Construct from components
  fieldValue
  (
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const word& valueType,
    const bool loadFromFiles = false
  );
  //- Return a reference to the selected fieldValue
  static autoPtr<fieldValue> New
  (
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles = false,
    const bool output = true
  );
  //- Destructor
  virtual ~fieldValue();
  // Public Member Functions
    // Access
      //- Return the name of the geometric source
      inline const word& name() const;
      //- Return the reference to the object registry
      inline const objectRegistry& obr() const;
      //- Return the reference to the construction dictionary
      inline const dictionary& dict() const;
      //- Return the active flag
      inline bool active() const;
      //- Return the switch to send output to Info as well as to file
      inline const Switch& log() const;
      //- Return the source name
      inline const word& sourceName() const;
      //- Return the list of field names
      inline const wordList& fields() const;
      //- Return the output field values flag
      inline const Switch& valueOutput() const;
      //- Helper function to return the reference to the mesh
      inline const fvMesh& mesh() const;
      //- Return access to the latest set of results
      inline const dictionary& resultDict() const;
    // Function object functions
      //- Read from dictionary
      virtual void read(const dictionary& dict);
      //- Write to screen/file
      virtual void write();
      //- Execute
      virtual void execute();
      //- Execute the at the final time-loop, currently does nothing
      virtual void end();
      //- Called when time was set at the end of the Time::operator++
      virtual void timeSet();
      //- Update mesh
      virtual void updateMesh(const mapPolyMesh&);
      //- Move points
      virtual void movePoints(const polyMesh&);
      //- Combine fields from all processor domains into single field
      template<class Type>
      void combineFields(Field<Type>& field);
      //- Combine fields from all processor domains into single field
      template<class Type>
      void combineFields(tmp<Field<Type> >&);
};
}  // namespace mousse
#include "field_value_i.hpp"
#ifdef NoRepository
  #include "field_value_templates.cpp"
#endif
#endif
