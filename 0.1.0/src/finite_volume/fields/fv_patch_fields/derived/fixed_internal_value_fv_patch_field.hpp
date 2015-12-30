// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedInternalValueFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition provides a mechanism to set boundary (cell) values
//   directly into a matrix, i.e. to set a constraint condition.  Default
//   behaviour is to act as a zero gradient condition.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedInternalValue;
//     value           uniform 0;              // place holder
//   }
//   \endverbatim
// Note
//   This is used as a base for conditions such as the turbulence \c epsilon
//   wall function, which applies a near-wall constraint for high Reynolds
//   number flows.
// SeeAlso
//   mousse::epsilonWallFunctionFvPatchScalarField
// SourceFiles
//   fixed_internal_value_fv_patch_field.cpp
#ifndef fixed_internal_value_fv_patch_field_hpp_
#define fixed_internal_value_fv_patch_field_hpp_
#include "zero_gradient_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class fixedInternalValueFvPatchField
:
  public zeroGradientFvPatchField<Type>
{
public:
  //- Runtime type information
  TypeName("fixedInternalValue");
  // Constructors
    //- Construct from patch and internal field
    fixedInternalValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedInternalValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given fixedInternalValueFvPatchField<Type>
    //  onto a new patch
    fixedInternalValueFvPatchField
    (
      const fixedInternalValueFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedInternalValueFvPatchField
    (
      const fixedInternalValueFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new fixedInternalValueFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedInternalValueFvPatchField
    (
      const fixedInternalValueFvPatchField<Type>&,
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
        new fixedInternalValueFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //-Manipulate a matrix
      virtual void manipulateMatrix(fvMatrix<Type>& matrix);
};
}  // namespace mousse
#ifdef NoRepository
#   include "fixed_internal_value_fv_patch_field.cpp"
#endif
#endif
