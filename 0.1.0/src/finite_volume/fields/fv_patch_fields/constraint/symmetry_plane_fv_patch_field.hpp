// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPlaneFvPatchField
// Group
//   grpConstraintBoundaryConditions
// Description
//   This boundary condition enforces a symmetryPlane constraint
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            symmetryPlane;
//   }
//   \endverbatim
// SourceFiles
//   symmetry_plane_fv_patch_field.cpp
//   symmetry_plane_fv_patch_fields.cpp
//   symmetry_plane_fv_patch_fields.hpp
//   symmetry_plane_fv_patch_fields_fwd.hpp
#ifndef symmetry_plane_fv_patch_field_hpp_
#define symmetry_plane_fv_patch_field_hpp_
#include "basic_symmetry_fv_patch_field.hpp"
#include "symmetry_plane_fv_patch.hpp"
namespace mousse
{
template<class Type>
class symmetryPlaneFvPatchField
:
  public basicSymmetryFvPatchField<Type>
{
  // Private data
    //- Local reference cast into the symmetryPlane patch
    const symmetryPlaneFvPatch& symmetryPlanePatch_;
public:
  //- Runtime type information
  TypeName(symmetryPlaneFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    symmetryPlaneFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    symmetryPlaneFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given symmetryPlaneFvPatchField
    //  onto a new patch
    symmetryPlaneFvPatchField
    (
      const symmetryPlaneFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    symmetryPlaneFvPatchField
    (
      const symmetryPlaneFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new symmetryPlaneFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    symmetryPlaneFvPatchField
    (
      const symmetryPlaneFvPatchField<Type>&,
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
        new symmetryPlaneFvPatchField<Type>(*this, iF)
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
tmp<scalarField> symmetryPlaneFvPatchField<scalar>::snGrad() const;
template<>
void symmetryPlaneFvPatchField<scalar>::evaluate
(
  const Pstream::commsTypes commsType
);
}  // namespace mousse
#ifdef NoRepository
#   include "symmetry_plane_fv_patch_field.cpp"
#endif
#endif
