#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FIELD_VALUE_FIELD_VALUE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FIELD_VALUE_FIELD_VALUE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldValue
// Group
//   grpFieldFunctionObjects
// Description
//   Base class for field value -based function objects.

#include "function_object_file.hpp"
#include "switch.hpp"
#include "ofstream.hpp"
#include "dictionary.hpp"
#include "field.hpp"
#include "run_time_selection_tables.hpp"
#include "fv_mesh.hpp"


namespace mousse {

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
      void combineFields(tmp<Field<Type>>&);
};

}  // namespace mousse


// Member Functions 
inline const mousse::word& mousse::fieldValue::name() const
{
  return name_;
}


inline const mousse::objectRegistry& mousse::fieldValue::obr() const
{
  return obr_;
}


inline const mousse::dictionary& mousse::fieldValue::dict() const
{
  return dict_;
}


inline bool mousse::fieldValue::active() const
{
  return active_;
}


inline const mousse::Switch& mousse::fieldValue::log() const
{
  return log_;
}


inline const mousse::word& mousse::fieldValue::sourceName() const
{
  return sourceName_;
}


inline const mousse::wordList& mousse::fieldValue::fields() const
{
  return fields_;
}


inline const mousse::Switch& mousse::fieldValue::valueOutput() const
{
  return valueOutput_;
}


inline const mousse::fvMesh& mousse::fieldValue::mesh() const
{
  return refCast<const fvMesh>(obr_);
}


inline const mousse::dictionary& mousse::fieldValue::resultDict() const
{
  return resultDict_;
}

#include "field_value.ipp"

#endif
