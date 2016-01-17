// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::addSubtract
// Description
//   adds/subtracts a field or value to/from a base field.
//   New field name specified by -resultName option, or automatically as:
//     \<baseFieldName\>_add_<addSubtractFieldName>
//     \<baseFieldName\>_add_value
//     \<baseFieldName\>_subtract_<addSubtractFieldName>
//     \<baseFieldName\>_subtract_value
//   Example usage:
//     addSubtract p add -value 100000 -resultName pAbs
//     addSubtract U subtract -field U0
// SourceFiles
//   add_subtract.cpp
//   writeadd_subtract_field.cpp
//   writeadd_subtract_value.cpp
#ifndef add_subtract_hpp_
#define add_subtract_hpp_
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class addSubtract
:
  public calcType
{
public:
  enum calcTypes
  {
    FIELD,
    VALUE
  };
  enum calcModes
  {
    ADD,
    SUBTRACT
  };
private:
  // Private data
    //- Name of base field (to addSubtract to)
    word baseFieldName_;
    //- Calc type as given by enumerations above
    calcTypes calcType_;
    //- Name of field to add/subtract
    word addSubtractFieldName_;
    //- String representation of value to add/subtract
    string addSubtractValueStr_;
    //- Name of result field
    word resultName_;
    //- Mode - addSubtract/subtract
    calcModes calcMode_;
  // Private Member Functions
    // Output
      //- Calc and output field addSubtractitions
      void writeAddSubtractFields
      (
        const Time& runTime,
        const fvMesh& mesh,
        const IOobject& baseFieldHeader
      );
      //- Calc and output field and value addSubtractitions
      void writeAddSubtractValues
      (
        const Time& runTime,
        const fvMesh& mesh,
        const IOobject& baseFieldHeader
      );
protected:
  // Member Functions
    // Calculation routines
      //- Initialise - typically setting static variables,
      //  e.g. command line arguments
      virtual void init();
      //- Pre-time loop calculations
      virtual void preCalc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
      //- Time loop calculations
      virtual void calc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
    // I-O
      //- Write addSubtract field
      template<class Type>
      void writeAddSubtractField
      (
        const IOobject& baseHeader,
        const IOobject& addSubtractHeader,
        const fvMesh& mesh,
        bool& processed
      );
      //- Write addSubtract value
      template<class Type>
      void writeAddSubtractValue
      (
        const IOobject& baseHeader,
        const string& valueStr,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TYPE_NAME("addSubtract");
  // Constructors
    //- Construct null
    addSubtract();
    //- Disallow default bitwise copy construct
    addSubtract(const addSubtract&) = delete;
    //- Disallow default bitwise assignment
    addSubtract& operator=(const addSubtract&) = delete;
  //- Destructor
  virtual ~addSubtract();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_add_subtract_field.cpp"
#   include "write_add_subtract_value.cpp"
#endif
#endif
