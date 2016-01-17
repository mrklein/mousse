// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::slipPointPatchField
// Description
//   mousse::slipPointPatchField
// SourceFiles
//   slip_point_patch_field.cpp
#ifndef slip_point_patch_field_hpp_
#define slip_point_patch_field_hpp_
#include "basic_symmetry_point_patch_field.hpp"
namespace mousse
{
template<class Type>
class slipPointPatchField
:
  public basicSymmetryPointPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("slip");
  // Constructors
    //- Construct from patch and internal field
    slipPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    slipPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    slipPointPatchField
    (
      const slipPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new slipPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    slipPointPatchField
    (
      const slipPointPatchField<Type>&,
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
        new slipPointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#    include "slip_point_patch_field.cpp"
#endif
#endif
