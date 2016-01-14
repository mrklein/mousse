// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldMinMax
// Group
//   grpFieldFunctionObjects
// Description
//   This function object calculates the value and location of scalar minimim
//   and maximum for a list of user-specified fields.  For variables with a rank
//   greater than zero, either the min/max of a component value or the magnitude
//   is reported.  When operating in parallel, the processor owning the value
//   is also given.
//   Example of function object specification:
//   \verbatim
//   fieldMinMax1
//   {
//     type        fieldMinMax;
//     functionObjectLibs ("libfieldFunctionObjects.so");
//     ...
//     write       yes;
//     log         yes;
//     location    yes;
//     mode        magnitude;
//     fields
//     (
//       U
//       p
//     );
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: fieldMinMax  | yes         |
//     write        | write min/max data to file |  no      | yes
//     log          | write min/max data to standard output | no | no
//     location     | write location of the min/max value | no | yes
//     mode         | calculation mode: magnitude or component | no | magnitude
//   \endtable
//   Output data is written to the file \<timeDir\>/fieldMinMax.dat
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   field_min_max.cpp
//   i_ofield_min_max.hpp
#ifndef field_min_max_hpp_
#define field_min_max_hpp_
#include "function_object_file.hpp"
#include "switch.hpp"
#include "vector.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class fieldMinMax
:
  public functionObjectFile
{
public:
  enum modeType
  {
    mdMag,
    mdCmpt
  };
protected:
  // Protected data
    //- Mode type names
    static const NamedEnum<modeType, 2> modeTypeNames_;
    //- Name of this set of field min/max
    //  Also used as the name of the output directory
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Switch to send output to Info as well
    Switch log_;
    //- Switch to write location of min/max values
    Switch location_;
    //- Mode for min/max - only applicable for ranks > 0
    modeType mode_;
    //- Fields to assess min/max
    wordList fieldSet_;
  // Private Member Functions
    //- Helper function to write the output
    template<class Type>
    void output
    (
      const word& fieldName,
      const word& outputName,
      const vector& minC,
      const vector& maxC,
      const label minProcI,
      const label maxProcI,
      const Type& minValue,
      const Type& maxValue
    );
    //- Disallow default bitwise copy construct
    fieldMinMax(const fieldMinMax&);
    //- Disallow default bitwise assignment
    void operator=(const fieldMinMax&);
    //- Output file header information
    virtual void writeFileHeader(const label i);
public:
  //- Runtime type information
  TYPE_NAME("fieldMinMax");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    fieldMinMax
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~fieldMinMax();
  // Member Functions
    //- Return name of the set of field min/max
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the field min/max data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the field min/max
    template<class Type>
    void calcMinMaxFields
    (
      const word& fieldName,
      const modeType& mode
    );
    //- Write the fieldMinMax
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#ifdef NoRepository
#   include "field_min_max_templates.cpp"
#endif
#endif
