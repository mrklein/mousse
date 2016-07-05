// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fixed_value_fv_patch_field_template.cpp
#ifndef fixed_value_fv_patch_template${FieldType}_hpp
#define fixed_value_fv_patch_template${FieldType}_hpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class ${typeName}FixedValueFvPatch${FieldType}
:
  public fixedValueFvPatchField<${TemplateType}>
{
public:
  //- Information about the SHA1 of the code itself
  static const char* const SHA1sum;
  //- Runtime type information
  TypeName("${typeName}");
  // Constructors
    //- Construct from patch and internal field
    ${typeName}FixedValueFvPatch${FieldType}
    (
      const fvPatch&,
      const DimensionedField<${TemplateType}, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    ${typeName}FixedValueFvPatch${FieldType}
    (
      const fvPatch&,
      const DimensionedField<${TemplateType}, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping a copy onto a new patch
    ${typeName}FixedValueFvPatch${FieldType}
    (
      const ${typeName}FixedValueFvPatch${FieldType}&,
      const fvPatch&,
      const DimensionedField<${TemplateType}, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    ${typeName}FixedValueFvPatch${FieldType}
    (
      const ${typeName}FixedValueFvPatch${FieldType}&
    );
    //- Construct and return a clone
    virtual tmp< fvPatch${FieldType} > clone() const
    {
      return tmp< fvPatch${FieldType} >
      (
        new ${typeName}FixedValueFvPatch${FieldType}(*this)
      );
    }
    //- Construct as copy setting internal field reference
    ${typeName}FixedValueFvPatch${FieldType}
    (
      const ${typeName}FixedValueFvPatch${FieldType}&,
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
        new ${typeName}FixedValueFvPatch${FieldType}(*this, iF)
      );
    }
  //- Destructor
  virtual ~${typeName}FixedValueFvPatch${FieldType}();
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
};
}  // namespace mousse
#endif
