// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zeroGradientPointPatchField
// Description
//   mousse::zeroGradientPointPatchField
// SourceFiles
//   zero_gradient_point_patch_field.cpp
#ifndef zero_gradient_point_patch_field_hpp_
#define zero_gradient_point_patch_field_hpp_

#include "point_patch_field.hpp"

namespace mousse
{
template<class Type>
class zeroGradientPointPatchField
:
  public pointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("zeroGradient");
  // Constructors
    //- Construct from patch and internal field
    zeroGradientPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    zeroGradientPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    zeroGradientPointPatchField
    (
      const zeroGradientPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new zeroGradientPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    zeroGradientPointPatchField
    (
      const zeroGradientPointPatchField<Type>&,
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
        new zeroGradientPointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#    include "zero_gradient_point_patch_field.cpp"
#endif
#endif
