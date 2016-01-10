// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicSymmetryPointPatchField
// Description
//   A Symmetry boundary condition for pointField
// SourceFiles
//   basicsymmetry_point_patch_field.cpp
#ifndef basic_symmetry_point_patch_field_hpp_
#define basic_symmetry_point_patch_field_hpp_
#include "point_patch_field.hpp"
#include "symmetry_point_patch.hpp"
namespace mousse
{
template<class Type>
class basicSymmetryPointPatchField
:
  public pointPatchField<Type>
{
public:
  // Constructors
    //- Construct from patch and internal field
    basicSymmetryPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    basicSymmetryPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    basicSymmetryPointPatchField
    (
      const basicSymmetryPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new basicSymmetryPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    basicSymmetryPointPatchField
    (
      const basicSymmetryPointPatchField<Type>&,
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
        new basicSymmetryPointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
};
}  // namespace mousse
#ifdef NoRepository
#    include "basic_symmetry_point_patch_field.cpp"
#endif
#endif
