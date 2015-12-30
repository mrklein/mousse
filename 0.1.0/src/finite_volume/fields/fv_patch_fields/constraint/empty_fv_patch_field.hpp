// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::emptyFvPatchField
// Group
//   grpConstraintBoundaryConditions
// Description
//   This boundary condition provides an 'empty' condition for reduced
//   dimensions cases, i.e. 1- and 2-D geometries.  Apply this condition to
//   patches whose normal is aligned to geometric directions that do not
//   constitue solution directions.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            empty;
//   }
//   \endverbatim
// SourceFiles
//   empty_fv_patch_field.cpp
#ifndef empty_fv_patch_field_hpp_
#define empty_fv_patch_field_hpp_
#include "fv_patch_field.hpp"
#include "empty_fv_patch.hpp"
namespace mousse
{
template<class Type>
class emptyFvPatchField
:
  public fvPatchField<Type>
{
public:
  //- Runtime type information
  TypeName(emptyFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    emptyFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    emptyFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given emptyFvPatchField onto a new patch
    emptyFvPatchField
    (
      const emptyFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    emptyFvPatchField
    (
      const emptyFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new emptyFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    emptyFvPatchField
    (
      const emptyFvPatchField<Type>&,
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
        new emptyFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      )
      {}
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchField<Type>&,
        const labelList&
      )
      {}
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      //  This only checks to see the case is actually 1D or 2D
      //  for which this boundary condition is valid
      void updateCoeffs();
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type> > valueInternalCoeffs
      (
        const tmp<scalarField>&
      ) const
      {
        return tmp<Field<Type> >(new Field<Type>(0));
      }
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type> > valueBoundaryCoeffs
      (
        const tmp<scalarField>&
      ) const
      {
        return tmp<Field<Type> >(new Field<Type>(0));
      }
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      tmp<Field<Type> > gradientInternalCoeffs() const
      {
        return tmp<Field<Type> >(new Field<Type>(0));
      }
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      tmp<Field<Type> > gradientBoundaryCoeffs() const
      {
        return tmp<Field<Type> >(new Field<Type>(0));
      }
};
}  // namespace mousse
#ifdef NoRepository
#   include "empty_fv_patch_field.cpp"
#endif
#endif
