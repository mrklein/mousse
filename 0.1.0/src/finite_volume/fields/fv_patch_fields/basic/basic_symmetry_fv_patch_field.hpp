// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicSymmetryFvPatchField
// Group
//   grpConstraintBoundaryConditions
// Description
//   A symmetry patch
// SourceFiles
//   basic_symmetry_fv_patch_field.cpp
#ifndef basic_symmetry_fv_patch_field_hpp_
#define basic_symmetry_fv_patch_field_hpp_
#include "transform_fv_patch_field.hpp"
#include "symmetry_fv_patch.hpp"
namespace mousse
{
template<class Type>
class basicSymmetryFvPatchField
:
  public transformFvPatchField<Type>
{
public:
  // Constructors
    //- Construct from patch and internal field
    basicSymmetryFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    basicSymmetryFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given basicSymmetryFvPatchField onto a new
    //  patch
    basicSymmetryFvPatchField
    (
      const basicSymmetryFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    basicSymmetryFvPatchField
    (
      const basicSymmetryFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new basicSymmetryFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    basicSymmetryFvPatchField
    (
      const basicSymmetryFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type> >
      (
        new basicSymmetryFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Return gradient at boundary
      virtual tmp<Field<Type> > snGrad() const;
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
      //- Return face-gradient transform diagonal
      virtual tmp<Field<Type> > snGradTransformDiag() const;
};
// Template Specialisations 
template<>
tmp<scalarField > basicSymmetryFvPatchField<scalar>::snGrad() const;
template<>
void basicSymmetryFvPatchField<scalar>::evaluate
(
  const Pstream::commsTypes commsType
);
}  // namespace mousse
#ifdef NoRepository
#   include "basic_symmetry_fv_patch_field.cpp"
#endif
#endif
