// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   mixed_fv_patch_field_template.cpp
#ifndef mixed_fv_patch_template_${FieldType}_hpp_
#define mixed_fv_patch_template_${FieldType}_hpp_
#include "mixed_fv_patch_fields.hpp"
namespace mousse
{
class ${typeName}MixedValueFvPatch${FieldType}
:
  public mixedFvPatchField<${TemplateType}>
{
public:
  //- Information about the SHA1 of the code itself
  static const char* const SHA1sum;
  //- Runtime type information
  TypeName("${typeName}");
  // Constructors
    //- Construct from patch and internal field
    ${typeName}MixedValueFvPatch${FieldType}
    (
      const fvPatch&,
      const DimensionedField<${TemplateType}, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    ${typeName}MixedValueFvPatch${FieldType}
    (
      const fvPatch&,
      const DimensionedField<${TemplateType}, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping a copy onto a new patch
    ${typeName}MixedValueFvPatch${FieldType}
    (
      const ${typeName}MixedValueFvPatch${FieldType}&,
      const fvPatch&,
      const DimensionedField<${TemplateType}, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    ${typeName}MixedValueFvPatch${FieldType}
    (
      const ${typeName}MixedValueFvPatch${FieldType}&
    );
    //- Construct and return a clone
    virtual tmp< fvPatch${FieldType} > clone() const
    {
      return tmp< fvPatch${FieldType} >
      (
        new ${typeName}MixedValueFvPatch${FieldType}(*this)
      );
    }
    //- Construct as copy setting internal field reference
    ${typeName}MixedValueFvPatch${FieldType}
    (
      const ${typeName}MixedValueFvPatch${FieldType}&,
      const DimensionedField<${TemplateType}, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp< fvPatch${FieldType} > clone
    (
      const DimensionedField<${TemplateType}, volMesh>& iF
    ) const
    {
      return tmp< fvPatch${FieldType} >
      (
        new ${typeName}MixedValueFvPatch${FieldType}(*this, iF)
      );
    }
  //- Destructor
  virtual ~${typeName}MixedValueFvPatch${FieldType}();
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
};
}  // namespace mousse
#endif
