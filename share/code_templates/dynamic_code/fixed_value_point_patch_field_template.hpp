// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fixed_value_point_patch_field_template.cpp
#ifndef fixed_value_point_patch_template_${FieldType}_hpp_
#define fixed_value_point_patch_template_${FieldType}_hpp_
#include "fixed_value_point_patch_fields.hpp"
namespace mousse
{
class ${typeName}FixedValuePointPatch${FieldType}
:
  public fixedValuePointPatchField<${TemplateType}>
{
public:
  //- Information about the SHA1 of the code itself
  static const char* const SHA1sum;
  //- Runtime type information
  TypeName("${typeName}");
  // Constructors
    //- Construct from patch and internal field
    ${typeName}FixedValuePointPatch${FieldType}
    (
      const pointPatch&,
      const DimensionedField<${TemplateType}, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    ${typeName}FixedValuePointPatch${FieldType}
    (
      const pointPatch&,
      const DimensionedField<${TemplateType}, pointMesh>&,
      const dictionary&,
      const bool valueRequired=true
    );
    //- Construct by mapping a copy onto a new patch
    ${typeName}FixedValuePointPatch${FieldType}
    (
      const ${typeName}FixedValuePointPatch${FieldType}&,
      const pointPatch&,
      const DimensionedField<${TemplateType}, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    ${typeName}FixedValuePointPatch${FieldType}
    (
      const ${typeName}FixedValuePointPatch${FieldType}&
    );
    //- Construct and return a clone
    virtual autoPtr< pointPatchField<${TemplateType}> > clone() const
    {
      return autoPtr< pointPatchField<${TemplateType}> >
      (
        new ${typeName}FixedValuePointPatch${FieldType}(*this)
      );
    }
    //- Construct as copy setting internal field reference
    ${typeName}FixedValuePointPatch${FieldType}
    (
      const ${typeName}FixedValuePointPatch${FieldType}&,
      const DimensionedField<${TemplateType}, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr< pointPatchField<${TemplateType}> > clone
    (
      const DimensionedField<${TemplateType}, pointMesh>& iF
    ) const
    {
      return autoPtr< pointPatchField<${TemplateType}> >
      (
        new ${typeName}FixedValuePointPatch${FieldType}(*this, iF)
      );
    }
  //- Destructor
  virtual ~${typeName}FixedValuePointPatch${FieldType}();
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
};
}  // namespace mousse
#endif
