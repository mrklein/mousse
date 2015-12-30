// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coded_fv_option_template.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "unit_conversion.hpp"
#include "fv_matrix.hpp"
//{{{ begin codeInclude
${codeInclude}
//}}} end codeInclude
namespace mousse
{
namespace fv
{
// Local Functions
//{{{ begin localCode
${localCode}
//}}} end localCode
// Global Functions 
extern "C"
{
  // dynamicCode:
  // SHA1 = ${SHA1sum}
  //
  // unique function name that can be checked if the correct library version
  // has been loaded
  void ${typeName}_${SHA1sum}(bool load)
  {
    if (load)
    {
      // code that can be explicitly executed after loading
    }
    else
    {
      // code that can be explicitly executed before unloading
    }
  }
}
// Static Data Members
//makeRemovablePatchTypeField
//(
//    fvPatch${FieldType},
//    ${typeName}FvOption${SourceType}
//);
defineTypeNameAndDebug(${typeName}FvOption${SourceType}, 0);
addRemovableToRunTimeSelectionTable
(
  option,
  ${typeName}FvOption${SourceType},
  dictionary
);
const char* const ${typeName}FvOption${SourceType}::SHA1sum =
  "${SHA1sum}";
// Constructors 
${typeName}FvOption${SourceType}::
${typeName}FvOption${SourceType}
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  option(name, modelType, dict, mesh)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from components\n";
  }
}
// Destructor 
${typeName}FvOption${SourceType}::
~${typeName}FvOption${SourceType}()
{
  if (${verbose:-false})
  {
    Info<<"destroy ${typeName} sha1: ${SHA1sum}\n";
  }
}
// Member Functions 
void ${typeName}FvOption${SourceType}::correct
(
  GeometricField<${TemplateType}, fvPatchField, volMesh>& fld
)
{
  if (${verbose:-false})
  {
    Info<<"${typeName}FvOption${SourceType}::correct()\n";
  }
//{{{ begin code
  ${codeCorrect}
//}}} end code
}
void ${typeName}FvOption${SourceType}::addSup
(
  fvMatrix<${TemplateType}>& eqn,
  const label fieldI
)
{
  if (${verbose:-false})
  {
    Info<<"${typeName}FvOption${SourceType}::addSup()\n";
  }
//{{{ begin code
  ${codeAddSup}
//}}} end code
}
void ${typeName}FvOption${SourceType}::addSup
(
  const volScalarField& rho,
  fvMatrix<${TemplateType}>& eqn,
  const label fieldI
)
{
  if (${verbose:-false})
  {
    Info<<"${typeName}FvOption${SourceType}::addSup()\n";
  }
//{{{ begin code
  ${codeAddSup}
//}}} end code
}
void ${typeName}FvOption${SourceType}::setValue
(
  fvMatrix<${TemplateType}>& eqn,
  const label fieldI
)
{
  if (${verbose:-false})
  {
    Info<<"${typeName}FvOption${SourceType}::setValue()\n";
  }
//{{{ begin code
  ${codeSetValue}
//}}} end code
}
}  // namespace mousse
}  // namespace fv
