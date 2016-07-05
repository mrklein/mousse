// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_value_point_patch_field_template.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "point_patch_field_mapper.hpp"
#include "point_fields.hpp"
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
makePointPatchTypeField
(
  pointPatch${FieldType},
  ${typeName}FixedValuePointPatch${FieldType}
);
const char* const ${typeName}FixedValuePointPatch${FieldType}::SHA1sum =
  "${SHA1sum}";
// Constructors 
${typeName}FixedValuePointPatch${FieldType}::
${typeName}FixedValuePointPatch${FieldType}
(
  const pointPatch& p,
  const DimensionedField<${TemplateType}, pointMesh>& iF
)
:
  fixedValuePointPatchField<${TemplateType}>(p, iF)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/DimensionedField\n";
  }
}
${typeName}FixedValuePointPatch${FieldType}::
${typeName}FixedValuePointPatch${FieldType}
(
  const ${typeName}FixedValuePointPatch${FieldType}& ptf,
  const pointPatch& p,
  const DimensionedField<${TemplateType}, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<${TemplateType}>(ptf, p, iF, mapper)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/DimensionedField/mapper\n";
  }
}
${typeName}FixedValuePointPatch${FieldType}::
${typeName}FixedValuePointPatch${FieldType}
(
  const pointPatch& p,
  const DimensionedField<${TemplateType}, pointMesh>& iF,
  const dictionary& dict,
  const bool valueRequired
)
:
  fixedValuePointPatchField<${TemplateType}>(p, iF, dict, valueRequired)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " from patch/dictionary\n";
  }
}
${typeName}FixedValuePointPatch${FieldType}::
${typeName}FixedValuePointPatch${FieldType}
(
  const ${typeName}FixedValuePointPatch${FieldType}& ptf
)
:
  fixedValuePointPatchField<${TemplateType}>(ptf)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum}"
      " as copy\n";
  }
}
${typeName}FixedValuePointPatch${FieldType}::
${typeName}FixedValuePointPatch${FieldType}
(
  const ${typeName}FixedValuePointPatch${FieldType}& ptf,
  const DimensionedField<${TemplateType}, pointMesh>& iF
)
:
  fixedValuePointPatchField<${TemplateType}>(ptf, iF)
{
  if (${verbose:-false})
  {
    Info<<"construct ${typeName} sha1: ${SHA1sum} "
      "as copy/DimensionedField\n";
  }
}
// Destructor 
${typeName}FixedValuePointPatch${FieldType}::
~${typeName}FixedValuePointPatch${FieldType}()
{
  if (${verbose:-false})
  {
    Info<<"destroy ${typeName} sha1: ${SHA1sum}\n";
  }
}
// Member Functions 
void ${typeName}FixedValuePointPatch${FieldType}::updateCoeffs()
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
  this->fixedValuePointPatchField<${TemplateType}>::updateCoeffs();
}
}  // namespace mousse
