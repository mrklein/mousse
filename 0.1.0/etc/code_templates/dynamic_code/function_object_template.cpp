// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "function_object_template.hpp"
#include "time.hpp"
#include "fv_cfd.hpp"
#include "unit_conversion.hpp"
namespace mousse
{
// Static Data Members
defineTypeNameAndDebug(${typeName}FunctionObject, 0);
// Local Functions
//{{{ begin localCode
${localCode}
//}}} end localCode
// Private Member Functions 
const objectRegistry& ${typeName}FunctionObject::obr() const
{
  return obr_;
}
const fvMesh& ${typeName}FunctionObject::mesh() const
{
  return refCast<const fvMesh>(obr_);
}
// Constructors 
${typeName}FunctionObject::${typeName}FunctionObject
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool
)
:
  name_(name),
  obr_(obr)
{
  read(dict);
}
// Destructor 
${typeName}FunctionObject::~${typeName}FunctionObject()
{}
// Member Functions 
void ${typeName}FunctionObject::read(const dictionary& dict)
{
  if (${verbose:-false})
  {
    Info<<"read ${typeName} sha1: ${SHA1sum}\n";
  }
//{{{ begin code
  ${codeRead}
//}}} end code
}
void ${typeName}FunctionObject::execute()
{
  if (${verbose:-false})
  {
    Info<<"execute ${typeName} sha1: ${SHA1sum}\n";
  }
//{{{ begin code
  ${codeExecute}
//}}} end code
}
void ${typeName}FunctionObject::end()
{
  if (${verbose:-false})
  {
    Info<<"end ${typeName} sha1: ${SHA1sum}\n";
  }
//{{{ begin code
  ${codeEnd}
//}}} end code
}
void ${typeName}FunctionObject::timeSet()
{
  if (${verbose:-false})
  {
    Info<<"timeSet ${typeName} sha1: ${SHA1sum}\n";
  }
//{{{ begin codeTime
  ${codeTimeSet}
//}}} end code
}
void ${typeName}FunctionObject::write()
{
  if (${verbose:-false})
  {
    Info<<"write ${typeName} sha1: ${SHA1sum}\n";
  }
//{{{ begin code
  ${code}
//}}} end code
}
}  // namespace mousse
