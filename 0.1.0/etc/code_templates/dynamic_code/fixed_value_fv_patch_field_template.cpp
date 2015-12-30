// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_value_fv_patch_field_template.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "unit_conversion.hpp"
//{{{ begin codeInclude
${codeInclude}
//}}} end codeInclude
namespace mousse
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
makeRemovablePatchTypeField
(
  fvPatch${FieldType},
  ${typeName}FixedValueFvPatch${FieldType}
);
const char* const ${typeName}FixedValueFvPatch${FieldType}::SHA1sum =
  "${SHA1sum}";
// Constructors 
${typeName}FixedValueFvPatch${FieldType}::
${typeName}FixedValueFvPatch${FieldType}
(
  const fvPatch& p,
  const DimensionedField<${TemplateType}, volMesh>& iF
)
:
  fixedValueFvPatchField<${TemplateType}>(p, iF)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/DimensionedField\n";
  }
}
${typeName}FixedValueFvPatch${FieldType}::
${typeName}FixedValueFvPatch${FieldType}
(
  const ${typeName}FixedValueFvPatch${FieldType}& ptf,
  const fvPatch& p,
  const DimensionedField<${TemplateType}, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<${TemplateType}>(ptf, p, iF, mapper)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/DimensionedField/mapper\n";
  }
}
${typeName}FixedValueFvPatch${FieldType}::
${typeName}FixedValueFvPatch${FieldType}
(
  const fvPatch& p,
  const DimensionedField<${TemplateType}, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<${TemplateType}>(p, iF, dict)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/dictionary\n";
  }
}
${typeName}FixedValueFvPatch${FieldType}::
${typeName}FixedValueFvPatch${FieldType}
(
  const ${typeName}FixedValueFvPatch${FieldType}& ptf
)
:
  fixedValueFvPatchField<${TemplateType}>(ptf)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " as copy\n";
  }
}
${typeName}FixedValueFvPatch${FieldType}::
${typeName}FixedValueFvPatch${FieldType}
(
  const ${typeName}FixedValueFvPatch${FieldType}& ptf,
  const DimensionedField<${TemplateType}, volMesh>& iF
)
:
  fixedValueFvPatchField<${TemplateType}>(ptf, iF)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum} "
      "as copy/DimensionedField\n";
  }
}
// Destructor 
${typeName}FixedValueFvPatch${FieldType}::
~${typeName}FixedValueFvPatch${FieldType}()
{
  if (${verbose:-false})
  {
    Info<<"destroy ${typeName} sha1: ${SHA1sum}\n";
  }
}
// Member Functions 
void ${typeName}FixedValueFvPatch${FieldType}::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  if (${verbose:-false})
  {
    Info<<"updateCoeffs ${typeName} sha1: ${SHA1sum}\n";
  }
//{{{ begin code
  ${code}
//}}} end code
  this->fixedValueFvPatchField<${TemplateType}>::updateCoeffs();
}
}  // namespace mousse
