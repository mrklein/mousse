// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mixed_fv_patch_field_template.hpp"
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
  ${typeName}MixedValueFvPatch${FieldType}
);
const char* const ${typeName}MixedValueFvPatch${FieldType}::SHA1sum =
  "${SHA1sum}";
// Constructors 
${typeName}MixedValueFvPatch${FieldType}::
${typeName}MixedValueFvPatch${FieldType}
(
  const fvPatch& p,
  const DimensionedField<${TemplateType}, volMesh>& iF
)
:
  mixedFvPatchField<${TemplateType}>(p, iF)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/DimensionedField\n";
  }
}
${typeName}MixedValueFvPatch${FieldType}::
${typeName}MixedValueFvPatch${FieldType}
(
  const ${typeName}MixedValueFvPatch${FieldType}& ptf,
  const fvPatch& p,
  const DimensionedField<${TemplateType}, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchField<${TemplateType}>(ptf, p, iF, mapper)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/DimensionedField/mapper\n";
  }
}
${typeName}MixedValueFvPatch${FieldType}::
${typeName}MixedValueFvPatch${FieldType}
(
  const fvPatch& p,
  const DimensionedField<${TemplateType}, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchField<${TemplateType}>(p, iF, dict)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/dictionary\n";
  }
}
${typeName}MixedValueFvPatch${FieldType}::
${typeName}MixedValueFvPatch${FieldType}
(
  const ${typeName}MixedValueFvPatch${FieldType}& ptf
)
:
  mixedFvPatchField<${TemplateType}>(ptf)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " as copy\n";
  }
}
${typeName}MixedValueFvPatch${FieldType}::
${typeName}MixedValueFvPatch${FieldType}
(
  const ${typeName}MixedValueFvPatch${FieldType}& ptf,
  const DimensionedField<${TemplateType}, volMesh>& iF
)
:
  mixedFvPatchField<${TemplateType}>(ptf, iF)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum} "
      "as copy/DimensionedField\n";
  }
}
// Destructor 
${typeName}MixedValueFvPatch${FieldType}::
~${typeName}MixedValueFvPatch${FieldType}()
{
  if (${verbose:-false})
  {
    Info<<"destroy ${typeName} sha1: ${SHA1sum}\n";
  }
}
// Member Functions 
void ${typeName}MixedValueFvPatch${FieldType}::updateCoeffs()
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
  this->mixedFvPatchField<${TemplateType}>::updateCoeffs();
}
}  // namespace mousse
