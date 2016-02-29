#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_KQ_R_WALL_FUNCTIONS_KQ_R_WALL_FUNCTION_FV_PATCH_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_KQ_R_WALL_FUNCTIONS_KQ_R_WALL_FUNCTION_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kqRWallFunctionFvPatchField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a suitable condition for turbulence
//   \c k, \c q, and \c R fields for the case of high Reynolds number flow using
//   wall functions.
//   It is a simple wrapper around the zero-gradient condition.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            kqRWallFunction;
//   }
//   \endverbatim
// SeeAlso
//   mousse::zeroGradientFvPatchField
// SourceFiles
//   kq_r_wall_function_fv_patch_field.cpp
#include "zero_gradient_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class kqRWallFunctionFvPatchField
:
  public zeroGradientFvPatchField<Type>
{
  // Private Member Functions
    //- Check the type of the patch
    void checkType();
public:
  //- Runtime type information
  TYPE_NAME("kqRWallFunction");
  // Constructors
    //- Construct from patch and internal field
    kqRWallFunctionFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    kqRWallFunctionFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    // kqRWallFunctionFvPatchField
    //  onto a new patch
    kqRWallFunctionFvPatchField
    (
      const kqRWallFunctionFvPatchField&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    kqRWallFunctionFvPatchField
    (
      const kqRWallFunctionFvPatchField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new kqRWallFunctionFvPatchField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    kqRWallFunctionFvPatchField
    (
      const kqRWallFunctionFvPatchField&,
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
        new kqRWallFunctionFvPatchField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Evaluate the patchField
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::Pstream::blocking
      );
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "kq_r_wall_function_fv_patch_field.cpp"
#endif
#endif
