// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_subtract.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace calcTypes
{
DEFINE_TYPE_NAME_AND_DEBUG(addSubtract, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(calcType, addSubtract, dictionary);
}
}
// Private Member Functions 
void mousse::calcTypes::addSubtract::writeAddSubtractFields
(
  const Time& runTime,
  const fvMesh& mesh,
  const IOobject& baseFieldHeader
)
{
  bool processed = false;
  IOobject addSubtractFieldHeader
  {
    addSubtractFieldName_,
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  if (addSubtractFieldHeader.headerOk())
  {
    writeAddSubtractField<scalar>
    (
      baseFieldHeader,
      addSubtractFieldHeader,
      mesh,
      processed
    );
    writeAddSubtractField<vector>
    (
      baseFieldHeader,
      addSubtractFieldHeader,
      mesh,
      processed
    );
    writeAddSubtractField<sphericalTensor>
    (
      baseFieldHeader,
      addSubtractFieldHeader,
      mesh,
      processed
    );
    writeAddSubtractField<symmTensor>
    (
      baseFieldHeader,
      addSubtractFieldHeader,
      mesh,
      processed
    );
    writeAddSubtractField<tensor>
    (
      baseFieldHeader,
      addSubtractFieldHeader,
      mesh,
      processed
    );
    if (!processed)
    {
      FatalError
        << "Unable to process " << baseFieldName_
        << " + " << addSubtractFieldName_ << nl
        << "No call to addSubtract for fields of type "
        << baseFieldHeader.headerClassName() << " + "
        << addSubtractFieldHeader.headerClassName() << nl << nl
        << exit(FatalError);
    }
  }
  else
  {
    FATAL_ERROR_IN("calcTypes::addSubtract::writeAddSubtractFields()")
      << "Unable to read addSubtract field: " << addSubtractFieldName_
      << nl << exit(FatalError);
  }
}
void mousse::calcTypes::addSubtract::writeAddSubtractValues
(
  const Time& /*runTime*/,
  const fvMesh& mesh,
  const IOobject& baseFieldHeader
)
{
  bool processed = false;
  writeAddSubtractValue<scalar>
  (
    baseFieldHeader,
    addSubtractValueStr_,
    mesh,
    processed
  );
  writeAddSubtractValue<vector>
  (
    baseFieldHeader,
    addSubtractValueStr_,
    mesh,
    processed
  );
  writeAddSubtractValue<sphericalTensor>
  (
    baseFieldHeader,
    addSubtractValueStr_,
    mesh,
    processed
  );
  writeAddSubtractValue<symmTensor>
  (
    baseFieldHeader,
    addSubtractValueStr_,
    mesh,
    processed
  );
  writeAddSubtractValue<tensor>
  (
    baseFieldHeader,
    addSubtractValueStr_,
    mesh,
    processed
  );
  if (!processed)
  {
    FATAL_ERROR_IN("calcTypes::addSubtract::writeAddSubtractValue()")
      << "Unable to process " << baseFieldName_
      << " + " << addSubtractValueStr_ << nl
      << "No call to addSubtract for fields of type "
      << baseFieldHeader.headerClassName() << nl << nl
      << exit(FatalError);
  }
}
// Constructors 
mousse::calcTypes::addSubtract::addSubtract()
:
  calcType(),
  baseFieldName_(""),
  calcType_(FIELD),
  addSubtractFieldName_(""),
  addSubtractValueStr_(""),
  resultName_(""),
  calcMode_(ADD)
{}
// Destructor 
mousse::calcTypes::addSubtract::~addSubtract()
{}
// Member Functions 
void mousse::calcTypes::addSubtract::init()
{
  argList::validArgs.append("add");
  argList::validArgs.append("baseField");
  argList::validArgs.append("calcMode");
  argList::validOptions.insert("field", "fieldName");
  argList::validOptions.insert("value", "valueString");
  argList::validOptions.insert("resultName", "fieldName");
}
void mousse::calcTypes::addSubtract::preCalc
(
  const argList& args,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{
  baseFieldName_ = args[2];
  const word calcModeName = args[3];
  if (calcModeName == "add")
  {
    calcMode_ = ADD;
  }
  else if (calcModeName == "subtract")
  {
    calcMode_ = SUBTRACT;
  }
  else
  {
    FATAL_ERROR_IN("calcTypes::addSubtract::preCalc")
      << "Invalid calcMode: " << calcModeName << nl
      << "    Valid calcModes are add and subtract" << nl
      << exit(FatalError);
  }
  if (args.optionReadIfPresent("field", addSubtractFieldName_))
  {
    calcType_ = FIELD;
  }
  else if (args.optionReadIfPresent("value", addSubtractValueStr_))
  {
    calcType_ = VALUE;
  }
  else
  {
    FATAL_ERROR_IN("calcTypes::addSubtract::preCalc")
      << "addSubtract requires either -field or -value option"
      << nl << exit(FatalError);
  }
  args.optionReadIfPresent("resultName", resultName_);
}
void mousse::calcTypes::addSubtract::calc
(
  const argList& /*args*/,
  const Time& runTime,
  const fvMesh& mesh
)
{
  IOobject baseFieldHeader
  (
    baseFieldName_,
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  );
  if (baseFieldHeader.headerOk())
  {
    switch (calcType_)
    {
      case FIELD:
      {
        writeAddSubtractFields(runTime, mesh, baseFieldHeader);
        break;
      }
      case VALUE:
      {
        writeAddSubtractValues(runTime, mesh, baseFieldHeader);
        break;
      }
      default:
      {
        FATAL_ERROR_IN("calcTypes::addSubtract::calc")
          << "unknown calcType " << calcType_ << nl
          << abort(FatalError);
      }
    }
  }
  else
  {
    FATAL_ERROR_IN("calcTypes::addSubtract::calc")
      << "Unable to read base field: " << baseFieldName_
      << nl << exit(FatalError);
  }
}
