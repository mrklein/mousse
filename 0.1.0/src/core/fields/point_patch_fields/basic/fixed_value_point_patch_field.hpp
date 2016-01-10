// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedValuePointPatchField
// Description
//   A FixedValue boundary condition for pointField.
// SourceFiles
//   fixed_value_point_patch_field.cpp
#ifndef fixed_value_point_patch_field_hpp_
#define fixed_value_point_patch_field_hpp_
#include "value_point_patch_field.hpp"
namespace mousse
{
template<class Type>
class fixedValuePointPatchField
:
  public valuePointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("fixedValue");
  // Constructors
    //- Construct from patch and internal field
    fixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&,
      const bool valueRequired=true
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    fixedValuePointPatchField
    (
      const fixedValuePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new fixedValuePointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    fixedValuePointPatchField
    (
      const fixedValuePointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type> > clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new fixedValuePointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Access
      //- Return true if this patch field fixes a value
      virtual bool fixesValue() const
      {
        return true;
      }
  // Member operators
    // Disable assignment operators
      virtual void operator=(const Field<Type>&)
      {}
      virtual void operator=(const Type&)
      {}
};
}  // namespace mousse
#ifdef NoRepository
#    include "fixed_value_point_patch_field.cpp"
#endif
#endif
