// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wedgeFvPatchField
// Group
//   grpConstraintBoundaryConditions
// Description
//   This boundary condition is similar to the cyclic condition, except that
//   it is applied to 2-D geometries.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            wedge;
//   }
//   \endverbatim
// SeeAlso
//   mousse::cyclicFvPatchField
// SourceFiles
//   wedge_fv_patch_field.cpp
#ifndef wedge_fv_patch_field_hpp_
#define wedge_fv_patch_field_hpp_
#include "transform_fv_patch_field.hpp"
#include "wedge_fv_patch.hpp"
namespace mousse
{
template<class Type>
class wedgeFvPatchField
:
  public transformFvPatchField<Type>
{
public:
  //- Runtime type information
  TypeName(wedgeFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    wedgeFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    wedgeFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given wedgeFvPatchField onto a new patch
    wedgeFvPatchField
    (
      const wedgeFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    wedgeFvPatchField
    (
      const wedgeFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new wedgeFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    wedgeFvPatchField
    (
      const wedgeFvPatchField<Type>&,
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
        new wedgeFvPatchField<Type>(*this, iF)
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
tmp<scalarField > wedgeFvPatchField<scalar>::snGrad() const;
template<>
void wedgeFvPatchField<scalar>::evaluate
(
  const Pstream::commsTypes commsType
);
}  // namespace mousse
#ifdef NoRepository
#   include "wedge_fv_patch_field.cpp"
#endif
#endif
